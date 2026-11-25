#include "platform/dc/matrix.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_S1 || SHENMUE_IS_S2)

#include <cmath>

namespace shenmue { namespace matrix_stack {

// @verified - d
float FactorizeMatrix(Matrix4x4 *matrix, std::int8_t *permutation) {
    const bool current = matrix == 0;
    if (current)
        matrix = GetMatrix(0);
    float scale[4];
    float determinant = 0.0f;
    for (std::int32_t row = 0; row < 4; ++row) {
        permutation[row] = static_cast<std::int8_t>(row);
        float largest = 0.0f;
        for (std::int32_t column = 0; column < 4; ++column) {
            const float value = std::fabs(matrix->rows[row][column]);
            if (value > largest)
                largest = value;
        }
        if (largest == 0.0f)
            return determinant;
        scale[row] = 1.0f / largest;
    }
    determinant = 1.0f;
    std::int32_t pivotIndex;
    for (std::int32_t column = 0; column < 4; ++column) {
        float largest = -1.0f;
        for (std::int32_t row = column; row < 4; ++row) {
            const std::int32_t index = permutation[row];
            const float value = std::fabs(matrix->rows[index][column]) * scale[index];
            if (value > largest) {
                largest = value;
                pivotIndex = row;
            }
        }
        const std::int8_t pivotRow = permutation[pivotIndex];
        if (pivotIndex != column) {
            permutation[pivotIndex] = permutation[column];
            permutation[column] = pivotRow;
            determinant = -determinant;
        }
        const float pivot = matrix->rows[pivotRow][column];
        determinant *= pivot;
        if (pivot == 0.0f)
            return determinant;
        for (std::int32_t row = column + 1; row < 4; ++row) {
            const std::int8_t index = permutation[row];
            const float multiplier = matrix->rows[index][column] / pivot;
            matrix->rows[index][column] = multiplier;
            for (std::int32_t trailing = column + 1; trailing < 4; ++trailing) {
                const float product = multiplier * matrix->rows[pivotRow][trailing];
                matrix->rows[index][trailing] -= product;
            }
        }
    }
    if (current)
        LoadMatrix(matrix);
    return determinant;
}

// @verified - d
float InvertMatrix(Matrix4x4 *matrix) {
    const bool current = matrix == 0;
    if (current)
        matrix = GetMatrix(0);
    std::int8_t permutation[4];
    const float determinant = FactorizeMatrix(matrix, permutation);
    if (determinant == 0.0f)
        return determinant;
    Matrix4x4 inverse;
    for (std::int32_t column = 0; column < 4; ++column) {
        for (std::int32_t row = 0; row < 4; ++row) {
            float value = permutation[row] == column ? 1.0f : 0.0f;
            for (std::int32_t previous = 0; previous < row; ++previous) {
                const float product = matrix->rows[permutation[row]][previous] * inverse.rows[previous][column];
                value -= product;
            }
            inverse.rows[row][column] = value;
        }
        for (std::int32_t row = 3; row >= 0; --row) {
            float value = inverse.rows[row][column];
            for (std::int32_t next = row + 1; next < 4; ++next) {
                const float product = matrix->rows[permutation[row]][next] * inverse.rows[next][column];
                value -= product;
            }
            inverse.rows[row][column] = value / matrix->rows[permutation[row]][row];
        }
    }
    *matrix = inverse;
    if (current)
        LoadMatrix(matrix);
    return determinant;
}

} } // namespace shenmue::matrix_stack

#endif
