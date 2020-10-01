char *processTextForVectorization(char *doc)
{
    char *trimmed = trim(doc);
    char *lowercase = lower(trimmed);
    char *no_stops = removeStopwords(lowercase);
    char *no_sep = removeSentenceSeparators(no_stops);
    char *cleaned = (char *)malloc(sizeof(char) * INT_MAX);
    char *token1, *save1, *token2, *save2, *word;
    token1 = strtok_r(no_sep, "\n", &save1);
    while (token1 != NULL)
    {
        token2 = strtok_r(token1, " ", &save2);
        while (token2 != NULL)
        {
            word = stem(token2);
            strcat(cleaned, word);
            strcat(cleaned, " ");
            free(word);
            token2 = strtok_r(NULL, " ", &save2);
        }
        strcat(cleaned, "\n");
        token1 = strtok_r(NULL, "\n", &save1);
    }

    char *result = trim(cleaned);
    free(trimmed);
    free(lowercase);
    free(no_stops);
    free(no_sep);
    free(cleaned);
    return result;
}

char *summarize_K(char *sentences, char *punkt, double *rowsums, int m, int K)
{
    if (K > m)
        K = m;

    int *positions = (int *)malloc(sizeof(int) * m);
    for (int i = 0; i < m; i++)
        positions[i] = i;

    double temp1, temp2;
    for (int i = 0; i < m - 1; i++)
    {
        for (int j = 0; j < m - i - 1; j++)
        {
            if (rowsums[j] < rowsums[j + 1])
            {
                temp1 = rowsums[j];
                rowsums[j] = rowsums[j + 1];
                rowsums[j + 1] = temp1;

                temp2 = positions[j];
                positions[j] = positions[j + 1];
                positions[j + 1] = temp2;
            }
        }
    }

    int *topk = (int *)malloc(sizeof(int) * K);
    memset(topk, -1, K);
    for (int i = 0; i < K; i++)
        topk[i] = positions[i];

    for (int i = 0; i < K - 1; i++)
    {
        for (int j = 0; j < K - i - 1; j++)
        {
            if (topk[j] > topk[j + 1])
            {
                temp1 = topk[j];
                topk[j] = topk[j + 1];
                topk[j + 1] = temp1;
            }
        }
    }

    char *summary = (char *)calloc(INT_MAX, sizeof(char));
    char *token, *save, *line;
    int line_number = 0, pos = 0, length = 0;
    token = strtok_r(sentences, "\n", &save);
    while (token != NULL)
    {
        if (line_number == topk[pos])
        {
            line = trim(token);
            strcat(summary, line);
            length += strlen(line);
            summary[length++] = punkt[pos];
            summary[length++] = ' ';
            pos++;
            free(line);
        }
        line_number++;
        token = strtok_r(NULL, "\n", &save);
    }

    free(positions);
    free(topk);
    return summary;
}

char *summarize_avg(char *sentences, char *punkt, double *rowsums, int m)
{
    double avg = 0;
    for (int i = 0; i < m; i++)
        avg += rowsums[i];
    avg /= (double)m;

    char *summary = (char *)calloc(INT_MAX, sizeof(char));

    int pos = 0, length = 0;
    char *token, *save, *line;
    token = strtok_r(sentences, "\n", &save);
    while (token != NULL)
    {
        if (rowsums[pos] >= avg)
        {
            line = trim(token);
            strcat(summary, line);
            length += strlen(line);
            summary[length++] = punkt[pos];
            summary[length++] = ' ';
            free(line);
        }
        pos++;
        token = strtok_r(NULL, "\n", &save);
    }
    return summary;
}

double *getRowSums(double **M, int m, int n)
{
    double *rowsum = (double *)malloc(sizeof(double) * m);
    for (int row = 0; row < m; row++)
    {
        rowsum[row] = 0;
        for (int col = 0; col < n; col++)
            rowsum[row] += M[row][col];
    }
    return rowsum;
}

char *summarize(char *doc, bool avg, int K)
{
    char *preprocessed = processTextForVectorization(doc);
    char *sentences = removeSentenceSeparators(doc);
    char *punkt = getSentenceSeparators(doc);

    TFIDF *vectorizer = tfidfVectorizer();
    tfidfFitTransform(vectorizer, preprocessed, true, true);
    double *rowsums = getRowSums(vectorizer->matrix, vectorizer->docs, vectorizer->vocab_size);

    char *summary;
    if (avg)
        summary = summarize_avg(sentences, punkt, rowsums, vectorizer->docs);
    else
        summary = summarize_K(sentences, punkt, rowsums, vectorizer->docs, K);

    free(preprocessed);
    free(sentences);
    free(punkt);
    free(rowsums);
    return summary;
}