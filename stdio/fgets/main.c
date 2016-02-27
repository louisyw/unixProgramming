char *fgets(char *s, int n, FILE *stream){
	register int c;
	register char *cs;

	while(--n > 0 && getc(stream) != EOF){
		if ((*cs++ = c) == '\n')
			break;
	}
	*cs = '\0';
	return (c == EOF && cs == s) ? NULL : s ;
}
