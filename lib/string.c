int h_strlen(char *c)
{
    for (int i = 0;; i++)
    {
        if (*c == '\0')
            return i;
        c++;
    }
}
