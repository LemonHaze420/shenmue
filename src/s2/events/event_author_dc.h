
void HideAuthorCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowAuthorCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}
#if defined(SHENMUE_EVENT_AUTHOR_PROPS)

void HideAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 1);
}
#endif
