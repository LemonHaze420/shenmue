#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vkd3d_shader.h>

static int read_file(const char *path, struct vkd3d_shader_code *code) {
    FILE *file = fopen(path, "rb");
    long length;
    void *bytes;
    if (!file)
        return -1;
    if (fseek(file, 0, SEEK_END) || (length = ftell(file)) < 0 || fseek(file, 0, SEEK_SET)) {
        fclose(file);
        return -1;
    }
    bytes = malloc((size_t)length + 1);
    if (!bytes || fread(bytes, 1, (size_t)length, file) != (size_t)length) {
        free(bytes);
        fclose(file);
        return -1;
    }
    fclose(file);
    ((char *)bytes)[length] = 0;
    code->code = bytes;
    code->size = (size_t)length;
    return 0;
}

static int open_include(const char *name, bool local, const char *parent, void *context, struct vkd3d_shader_code *code) {
    const char *directory = context;
    size_t length = strlen(directory) + strlen(name) + 2;
    char *path = malloc(length);
    int result;
    (void)local;
    (void)parent;
    if (!path)
        return -1;
    snprintf(path, length, "%s/%s", directory, name);
    result = read_file(path, code);
    free(path);
    return result;
}

static void close_include(const struct vkd3d_shader_code *code, void *context) {
    (void)context;
    free((void *)code->code);
}

int main(int argc, char **argv) {
    struct vkd3d_shader_code source = {0}, compiled = {0};
    struct vkd3d_shader_macro macros[] = {{"SHENMUE_GFX_STANDALONE", "1"}, {"SHENMUE_GFX_DXVK_NATIVE", "1"}};
    struct vkd3d_shader_preprocess_info preprocess = {
        .type = VKD3D_SHADER_STRUCTURE_TYPE_PREPROCESS_INFO,
        .macros = macros,
        .macro_count = sizeof(macros) / sizeof(macros[0]),
        .pfn_open_include = open_include,
        .pfn_close_include = close_include,
    };
    struct vkd3d_shader_hlsl_source_info hlsl = {
        .type = VKD3D_SHADER_STRUCTURE_TYPE_HLSL_SOURCE_INFO,
        .next = &preprocess,
        .entry_point = "main",
    };
    struct vkd3d_shader_compile_info info = {
        .type = VKD3D_SHADER_STRUCTURE_TYPE_COMPILE_INFO,
        .next = &hlsl,
        .source_type = VKD3D_SHADER_SOURCE_HLSL,
        .target_type = VKD3D_SHADER_TARGET_DXBC_TPF,
        .log_level = VKD3D_SHADER_LOG_WARNING,
    };
    char *messages = NULL;
    FILE *output;
    int result;
    if (argc != 6) {
        fprintf(stderr, "Usage: %s source output identifier profile include-directory\n", argv[0]);
        return 2;
    }
    if (read_file(argv[1], &source)) {
        fprintf(stderr, "Cannot read shader: %s\n", argv[1]);
        return 1;
    }
    info.source = source;
    info.source_name = argv[1];
    hlsl.profile = argv[4];
    preprocess.include_context = argv[5];
    result = vkd3d_shader_compile(&info, &compiled, &messages);
    if (messages)
        fputs(messages, stderr);
    vkd3d_shader_free_messages(messages);
    free((void *)source.code);
    if (result < 0)
        return 1;
    output = fopen(argv[2], "wb");
    if (!output) {
        vkd3d_shader_free_shader_code(&compiled);
        return 1;
    }
    fprintf(output, "static const unsigned char %s[] = {\n", argv[3]);
    for (size_t i = 0; i < compiled.size; ++i)
        fprintf(output, "%s0x%02x,%s", i % 16 ? " " : "    ", ((const unsigned char *)compiled.code)[i], i % 16 == 15 ? "\n" : "");
    fputs("\n};\n", output);
    result = ferror(output);
    if (fclose(output))
        result = 1;
    vkd3d_shader_free_shader_code(&compiled);
    return result ? 1 : 0;
}
