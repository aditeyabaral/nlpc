/*
Store a word in the vocabulary,
its embedding vector and its one hot
representation 
*/
typedef struct node
{
    char* word;
    double** wordvector;
    int** onehotvector;
}NODE;

/*
Store model hyperparameters, trainable weights
and biases, corpus, cleaned corpus and the vocabulary. 
Vocabulary is stored in the form of a hash table
*/
typedef struct embedding
{
    /* HYPERPARAMETERS */
    int context;
    float alpha;
    int dimension;
    int vocab_size;
    int corpus_length;
    int batch_size;
    int epochs;

    /* WEIGHTS AND BIASES */
    double **W1;
    double **W2;
    double **b1;
    double **b2;

    /* INPUT AND OUTPUT MATRICES */
    double** X;
    double** Y;
    double** A1;
    double** A2;
    char* vocab;
    char* corpus;
    char* clean_corpus;

    /* HASH TABLE FOR VOCABULARY STORAGE */
    NODE **hashtable;
}EMBEDDING;

/* Structure for holding similarity info of a word relative to a specific word */
typedef struct similarity_info
{
    char* word;
    double sim;
}SIM_INFO;

void initialiseModelParameters(EMBEDDING* model, int C, int N, float alpha, int epochs)
{
    if (C > 0)
        model->context = C;

    if (N > 0)
        model->dimension = N;

    if (alpha > 0)
        model->alpha = alpha;

    if (epochs > 0)
        model->epochs = epochs;
}

void initialiseModelCorpus(EMBEDDING* model, char* corpus)
{
    int len = strlen(corpus);
    model->corpus_length = len;
    model->corpus = (char*)malloc(sizeof(char)*len);
    model->clean_corpus = (char*)malloc(sizeof(char)*len);
    
    char* no_punkt = remove_punctuations(corpus);
    char* cleaned_corpus = trim(no_punkt);
    strcpy(model->corpus, corpus);
    strcpy(model->clean_corpus, cleaned_corpus);
    
    free(no_punkt);
    free(cleaned_corpus);
}

void initialiseModelHashtable(EMBEDDING* model)
{
    model->hashtable = (NODE**)malloc(sizeof(NODE*)*model->vocab_size);
    for(int i = 0; i<model->vocab_size; i++)
        model->hashtable[i] = NULL;
}

void createXandY(EMBEDDING* model, int random_state)
{
    int num_words = 0;
    char* token1 , *save1;
    char* temp1 = (char*)malloc(sizeof(char)*model->corpus_length);
    strcpy(temp1, model->clean_corpus);
    token1 = strtok_r(temp1, " ", &save1);
    while(token1 != NULL)
    {
        num_words++;
        token1 = strtok_r(NULL, " ", &save1);
    }
    if ((2*model->context+1) >= num_words)
    {
        printf("Not enough words available for context. Window >= number of words.\n");
        return;
    }
    int ctr1 = 1, ctr2 = 1;
    char* temp2 = (char*)malloc(sizeof(char)*model->corpus_length);
    strcpy(temp1, model->clean_corpus);
    token1 = strtok_r(temp1, " ", &save1);
    char* token2, *save2;

    char* X_words = (char*)malloc(sizeof(char)*INT_MAX);
    char* y_words = (char*)malloc(sizeof(char)*INT_MAX);

    while(token1 != NULL && ctr1 <= (num_words-2*model->context))
    {
        ctr2 = 1;
        strcpy(temp2, model->clean_corpus);
        token2 = strtok_r(temp2, " ", &save2);
        while(token2 != NULL && ctr2!=ctr1)
        {
            token2 = strtok_r(NULL, " ", &save2);
            ctr2++;
        }
        for(int i=1;i<=model->context;i++)
        {
            strcat(X_words, token2);
            strcat(X_words, " ");
            token2 = strtok_r(NULL, " ", &save2);
        }
        strcat(y_words, token2);
        strcat(y_words, "\n");
        token2 = strtok_r(NULL, " ", &save2);
        for(int i=1;i<=model->context;i++)
        {
            strcat(X_words, token2);
            strcat(X_words, " ");
            token2 = strtok_r(NULL, " ", &save2);
        }
        
        X_words[strlen(X_words)-1] = '\n';
        ctr1++;
        token1 = strtok_r(NULL, " ", &save1);
    }

    int m = ctr1-1;
    model->batch_size = m;
    printf("Obtaining X matrix...\n");
    model->X = getX(model, m, X_words);
    printf("Obtaining y matrix...\n\n");
    model->Y = getY(model, m, y_words);

    free(temp1);
    free(temp2);
    free(X_words);
    free(y_words);
}

EMBEDDING* createModel()
{
    EMBEDDING* model = (EMBEDDING*)malloc(sizeof(EMBEDDING));

    model->context = 2;
    model->dimension = 100;
    model->alpha = 0.01;
    model->epochs = 100;
    model->batch_size = 0;
    model->corpus_length = 0;
    model->vocab_size = 0;
    model->A1 = NULL;
    model->b1 = NULL;
    model->b2 = NULL;
    model->clean_corpus = NULL;
    model->corpus = NULL;
    model->hashtable = NULL;
    model->vocab = NULL;
    model->W1 = NULL;
    model->W2 = NULL;
    model->X = NULL;
    model->Y = NULL;
    model->A2 = NULL;

    return model;
}

void extractEmbeddings(EMBEDDING* model)
{
    double** W2T = transpose(model->W2, model->vocab_size, model->dimension);
    double** W1_add_W2T = add(model->W1, W2T, model->dimension, model->vocab_size);
    double** W = multiply_scalar(W1_add_W2T, 0.5, model->dimension, model->vocab_size);
    for(int i=0; i< model-> vocab_size;i++)
    {
        if (model->hashtable[i]!=NULL)
        {
            if (model->hashtable[i]->wordvector == NULL)
                model->hashtable[i]->wordvector = createZerosArray(1, model->dimension);
            for(int j=0; j<model->dimension; j++)
                model->hashtable[i]->wordvector[0][j] = W[j][i];
        }
    }
    free2D(W2T, model->dimension, model->vocab_size);
    free2D(W1_add_W2T, model->dimension, model->vocab_size);
    free2D(W, model->dimension, model->vocab_size);
}

void train(EMBEDDING* model, char* corpus, int C, int N, float alpha, int epochs, int random_state, bool save)
{
    printf("Initialising hyperparameters...\n");
    initialiseModelParameters(model, C, N, alpha, epochs);
    
    if(corpus != NULL)
    {
        printf("Creating Vocabulary...\n");
        initialiseModelCorpus(model, corpus);
        createHashtable(model, corpus);
        
        model->W1 = createArray(model->dimension, model->vocab_size, random_state);
        model->W2 = createArray(model->vocab_size, model->dimension, random_state);
        model->b1 = createArray(model->dimension, 1, random_state);
        model->b2 = createArray(model->vocab_size, 1, random_state);
        
        printf("Calculating X and y...\n");
        createXandY(model, random_state);
    }
    
    printf("Initiating Training...\n");
    gradientDescent(model, save);
    extractEmbeddings(model);
}

double** getVector(EMBEDDING* model, char* word)
{
    int index = getHashvalue(word, model->vocab_size);
    int ctr = 0;
    while(model->hashtable[index] != NULL && ctr != model->vocab_size)
    {
        if(!strcmp(model->hashtable[index]->word, word))
            return model->hashtable[index]->wordvector;
        index = (index+1)%model->vocab_size;
        ctr+= 1;
    }
    return NULL;
}

char* getWord(EMBEDDING* model, double** vector)
{
    char* word = (char*)malloc(sizeof(char)*50);
    double max_sim = -1.0, sim;
    for(int i = 0; i < model->vocab_size; i++)
    {
        sim = cosine_similarity(vector, model->hashtable[i]->wordvector, model->dimension);
        if (sim > max_sim)
        {
            max_sim = sim;
            memset(word, '\0', 50);
            strcpy(word, model->hashtable[i]->word);
        }
    }
    return word;
}

char* mostSimilarByVector(EMBEDDING* model, double** vector, int k)
{
    SIM_INFO* sims = (SIM_INFO*)malloc(sizeof(SIM_INFO)*model->vocab_size);
    for(int i=0; i<model->vocab_size; ++i)
    {
        sims[i].word = (char*)malloc(sizeof(char)*50);
        strcpy(sims[i].word, model->hashtable[i]->word);
        sims[i].sim = cosine_similarity(model->hashtable[i]->wordvector, vector, model->dimension);
    }
    char* temp = (char*)malloc(sizeof(char)*50);
    double s;

    char* top_k_words = (char*)malloc(sizeof(char)*k*50);
    strcpy(top_k_words, "");
    for(int i=0; i<k; ++i)
    {
        for(int j=0; j<model->vocab_size; ++j)
        {
            if(sims[j].sim > sims[j+1].sim)
            {
                strcpy(temp, sims[j].word);
                strcpy(sims[j].word, sims[j+1].word);
                strcpy(sims[j+1].word, temp);

                s = sims[j].sim;
                sims[j].sim = sims[j+1].sim;
                sims[j+1].sim = s;
            }
        }
    }

    for(int t = model->vocab_size - k -1; t < model->vocab_size; ++t)
    {
        strcat(top_k_words, sims[t].word);
        strcat(top_k_words, "\n");
    }

    for(int i=0; i<model->vocab_size; ++i)
    {
        free(sims[i].word);
    }
    free(sims);
    free(temp);
    return top_k_words;
}

char* mostSimilarByWord(EMBEDDING* model, char* word, int k)
{
    double** word_vector = getVector(model, word);
    if(word_vector == NULL)
    {
        printf("%s not in vocabulary!\n", word);
        return NULL;
    }
    else
    {
        char* top_k_words = mostSimilarByVector(model, word_vector, k);
        free2D(word_vector, 1, model->dimension);
        return top_k_words;
    }
}

int getHashvalue(char *word, int vocab_size)
{
    int val = 0;
    int i = 0;
    while(word[i] != '\0')
    {
        val+= word[i]*(i+1);;
        i++;
    }
    val %= vocab_size;
    return val;
}

void insert(NODE* node, EMBEDDING* model)
{
    int index = getHashvalue(node->word, model->vocab_size);
    while(model->hashtable[index] != NULL)
    {
        if(!strcmp(model->hashtable[index]->word, node->word))
        {
            free(node->word);
            free2D_int(node->onehotvector, 1, model->vocab_size);
            free(node);
            return;
        }
        index = (index+1)%model->vocab_size;
    }
    model->hashtable[index] = node;
}

void createHashtable(EMBEDDING* model, char* corpus)
{
    model->vocab_size = getVocabularySize(model);
    initialiseModelHashtable(model);
    char word[30];
    char *save, *token;
    char *temp = (char*)malloc(sizeof(char)*strlen(model->vocab));
    strcpy(temp, model->vocab);
    token = strtok_r(temp, " ", &save);
    while (token != NULL)
    {
        strcpy(word, token);
        NODE* node = (NODE*)malloc(sizeof(NODE));
        node->word = (char*)malloc(sizeof(char)*strlen(word));
        strcpy(node->word, word);
        node->wordvector = NULL;
        node->onehotvector = createOneHot(node, model);
        insert(node, model);
        token = strtok_r(NULL, " ", &save);
    }
    free(temp);
}

bool checkFileExists(char* filename)
{
    if (filename == NULL)
        return false;
    FILE* fp = fopen(filename, "r");
    bool check = true;
    if (fp == NULL)
    {
        printf("%s not found.\n", filename);
        check = false;
    }
    fclose(fp);
    return check;
}

void getFileDimensions(char* filename, int *m, int *n)
{
    FILE* fp = fopen(filename, "r");
    char* temp = (char*)malloc(sizeof(char)*INT_MAX);
    char *line, *token, *save;
    *m = 0;
    *n = 0;
    bool flag = false;
    while(fgets(temp, INT_MAX, fp))
    {
        line = trim(temp);
        *m = *m + 1;
        token = strtok_r(line, ",", &save);
        while(token!=NULL && !flag)
        {
            *n = *n + 1;
            token = strtok_r(NULL, ",", &save);
        }
        flag = true;
        free(line);
    }
    free(temp);
    fclose(fp);
}

double** getMatrixFromFile(char* filename)
{
    int m, n, row, col;
    getFileDimensions(filename, &m, &n);
    printf("Shape = (%d, %d)\n", m, n);
    double** M = createZerosArray(m, n);

    FILE* fp = fopen(filename, "r");
    char* temp = (char*)malloc(sizeof(char)*INT_MAX);
    char *line, *token, *save, *ptr;
    row = 0;
    while(fgets(temp, INT_MAX, fp))
    {
        line = trim(temp);
        token = strtok_r(line, ",", &save);
        col = 0;
        while(token!=NULL)
        {
            M[row][col] = strtod(token, &ptr);
            col++;
            token = strtok_r(NULL, ",", &save);
        }
        row++;
        free(line);
    }
    free(temp);
    fclose(fp);
    return M;
}

void writeMatrixToFile(double** M, char* filename, int m, int n)
{
    FILE* fp  = fopen(filename, "w");
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
            fprintf(fp, "%lf,", M[i][j]);
        fprintf(fp, "%c", '\n');
    }
    fclose(fp);
}

void writeEmbeddings(EMBEDDING* model)
{
    FILE* fp  = fopen("model-embeddings.csv", "w");
    for(int i = 0; i < model->vocab_size; i++)
    {
        fprintf(fp, "%s,", model->hashtable[i]->word);
        for(int j = 0; j < model->dimension; j++)
            fprintf(fp, "%lf,", model->hashtable[i]->wordvector[0][j]);
        fprintf(fp, "%c", '\n');
    }
    fclose(fp);
    printf("Embedding saved...\n");
}

void writeParameters(EMBEDDING* model)
{
    FILE* fp  = fopen("model-parameters.csv", "w");
    fprintf(fp, "alpha,%f,\n", model->alpha);
    fprintf(fp, "C,%d,\n", model->context);
    fprintf(fp, "N,%d,\n", model->dimension);
    fprintf(fp, "Vocabulary Size,%d,\n", model->vocab_size);
    fprintf(fp, "Batch Size,%d,\n", model->batch_size);
    fprintf(fp, "Corpus Length,%d,\n", model->corpus_length);
    fprintf(fp, "Epochs,%d,\n", model->epochs);
    fclose(fp);
    printf("Parameters saved...\n");
}

void writeCorpus(EMBEDDING* model)
{
    FILE* fp  = fopen("model-corpus.txt", "w");
    fprintf(fp, "Cleaned Corpus,%s,\n\n", model->clean_corpus);
    fprintf(fp, "Vocabulary,%s,\n\n", model->vocab);
    fprintf(fp, "Corpus,%s,\n\n", model->corpus);
    fclose(fp);
    printf("Corpus saved...\n");
}

void writeXY(EMBEDDING* model)
{
    writeMatrixToFile(model->X, "model-X.csv", model->vocab_size, model->batch_size);
    writeMatrixToFile(model->Y, "model-y.csv", model->vocab_size, model->batch_size);
    printf("X and y saved...\n");
}

void writeWeightsBiases(EMBEDDING* model)
{
    writeMatrixToFile(model->W1, "model-weights-w1.csv", model->dimension, model->vocab_size);
    writeMatrixToFile(model->W2, "model-weights-w2.csv", model->vocab_size, model->dimension);
    printf("Weights saved...\n");
    writeMatrixToFile(model->b1, "model-bias-b1.csv", model->dimension, 1);
    writeMatrixToFile(model->b2, "model-bias-b2.csv", model->vocab_size, 1);
    printf("Biases saved...\n");
}

void saveModel(EMBEDDING* model, bool write_all)
{
    writeEmbeddings(model);
    writeParameters(model);
    writeWeightsBiases(model);
    writeXY(model);
    if (write_all)
        writeCorpus(model);
    printf("Model saved successfully...\n\n");
}


EMBEDDING* loadModelForTraining(char* embedding_filename, char* X_filename, char* Y_filename, char* W1_filename, char* W2_filename, char* b1_filename, char* b2_filename)
{
    printf("Loading model...\n");
    if(!(checkFileExists(X_filename) && checkFileExists(Y_filename) && checkFileExists(W1_filename) 
        && checkFileExists(W2_filename) && checkFileExists(b1_filename) && checkFileExists(b2_filename)))
    {
        printf("Essential file(s) missing. Aborting...\n");
        return NULL;
    }

    EMBEDDING* model = createModel();
    int m, n;
    if (checkFileExists(embedding_filename))
    {
        printf("Loading embeddings...\n");
        getFileDimensions(embedding_filename, &m, &n);
        model->vocab_size = m;
        model->dimension = n-1;
        printf("Shape = (%d, %d)\n", model->vocab_size, model->dimension);
        getEmbeddingParametersFromFile(model, embedding_filename);
    }

    getFileDimensions(X_filename, &m, &n);
    model->batch_size = m;

    printf("Loading X...\n");
    model->X = getMatrixFromFile(X_filename);
    printf("Loading y...\n");
    model->Y = getMatrixFromFile(Y_filename);
    printf("Loading W1...\n");
    model->W1 = getMatrixFromFile(W1_filename);
    printf("Loading W2...\n");
    model->W2 = getMatrixFromFile(W2_filename);
    printf("Loading b1...\n");
    model->b1 = getMatrixFromFile(b1_filename);
    printf("Loading b2...\n");
    model->b2 = getMatrixFromFile(b2_filename);
    printf("Model loaded successfully\n\n");
    return model;
}


void getEmbeddingParametersFromFile(EMBEDDING* model, char* filename)
{
    initialiseModelHashtable(model);
    FILE* fp = fopen(filename, "r");
    char* temp = (char*)malloc(sizeof(char)*INT_MAX);
    char *line, *token, *save, *ptr;
    char word[50];
    int pos = 0, dim = 0;
    while(fgets(temp, INT_MAX, fp))
    {
        line = trim(temp);
        token = strtok_r(line, ",", &save);
        strcpy(word, token);

        NODE* node = (NODE*)malloc(sizeof(NODE));
        node->word = (char*)malloc(sizeof(char)*strlen(word));
        strcpy(node->word, word);

        node->onehotvector = createOneHot(node, model);
        insert(node, model);

        node->wordvector = createZerosArray(1, model->dimension);
        token = strtok_r(NULL, ",", &save);
        dim = 0;
        while(token!=NULL)
        {
            double val = strtod(token, &ptr);
            node->wordvector[0][dim] = val;
            dim++;
            token = strtok_r(NULL, ",", &save);
        }
        free(line);
    }
    free(temp);
}

EMBEDDING* loadModelEmbeddings(char* embedding_filename)
{
    printf("Loading model...\n");
    if(!checkFileExists(embedding_filename))
    {
        printf("File missing. Aborting...\n");
        return NULL;
    }
    EMBEDDING* model = createModel();
    int m, n;
    getFileDimensions(embedding_filename, &m, &n);
    model->vocab_size = m;
    model->dimension = n-1;
    printf("Loading embeddings...\n");
    getEmbeddingParametersFromFile(model, embedding_filename);
    printf("Shape = (%d, %d)\n", model->vocab_size, model->dimension);
    printf("Model loaded successfully\n\n");
    return model;
}

double cost(EMBEDDING* model)
{
    double loss = 0;
    int m = model->batch_size;
    double sum;
    for (int i=0; i<m; i++)
    {
        sum = 0;
        for(int j = 0; j<model->vocab_size; j++)
            sum+= (model->Y[j][i])*log(model->A2[j][i]);
        loss+= sum;
    }
    loss = (-1.0/m)*loss;
    return loss;
}

void forward_propagation(EMBEDDING* model)
{
    if(model->A1 != NULL)
        free2D(model->A1, model->dimension, model->batch_size);
    if(model->A2 != NULL)
        free2D(model->A2, model->vocab_size, model->batch_size);

    double **W1X = multiply(model->W1, model->X, model->dimension, model->vocab_size, model->vocab_size, model->batch_size);
    double **Z1 = broadcast_and_add(W1X, model->b1, model->dimension, model->batch_size, model->dimension, 1);
    model->A1 = relu(Z1, model->dimension, model->batch_size);
    
    double **W2A1 = multiply(model->W2, model->A1, model->vocab_size, model->dimension, model->dimension, model->batch_size);
    double **Z2 = broadcast_and_add(W2A1, model->b2, model->vocab_size, model->batch_size, model->vocab_size, 1);
    model->A2 = softmax(Z2, model->vocab_size, model->batch_size, 1);
    
    free2D(W1X, model->dimension, model->batch_size);
    free2D(Z1, model->dimension, model->batch_size);
    free2D(W2A1, model->vocab_size, model->batch_size);
    free2D(Z2, model->vocab_size, model->batch_size);

}

void back_propagation(EMBEDDING* model)
{
    double **W2T = transpose(model->W2, model->vocab_size, model->dimension);
    double **yhat_diff_y = subtract(model->A2, model->Y, model->vocab_size, model->batch_size);
    double **W2T_mul_yhat_diff_y = multiply(W2T, yhat_diff_y, model->dimension, model->vocab_size, model->vocab_size, model->batch_size);
    double **relu_W2T_yhat_diff_y = relu(W2T_mul_yhat_diff_y, model->dimension, model->batch_size);
    double **OnesVector = createOnesArray(model->batch_size, 1);
    double ratio = 1.0/model->batch_size;

    double** XT = transpose(model->X, model->vocab_size, model->batch_size);
    double** dW1_partial_product = multiply(relu_W2T_yhat_diff_y, XT, model->dimension, model->batch_size, model->batch_size, model->vocab_size);
    double** dW1 = multiply_scalar(dW1_partial_product, ratio, model->dimension, model->vocab_size);

    double** AT = transpose(model->A1, model->dimension, model->batch_size);
    double** dW2_partial_product = multiply(yhat_diff_y, AT, model->vocab_size, model->batch_size, model->batch_size, model->dimension);
    double** dW2 = multiply_scalar(dW2_partial_product, ratio, model->vocab_size, model->dimension);

    double** db1_partial_product = multiply(relu_W2T_yhat_diff_y, OnesVector, model->dimension, model->batch_size, model->batch_size, 1);
    double** db1 = multiply_scalar(db1_partial_product, ratio, model->dimension, 1);

    double** db2_partial_product = multiply(yhat_diff_y, OnesVector, model->vocab_size, model->batch_size, model->batch_size, 1);
    double** db2 = multiply_scalar(db2_partial_product, ratio, model->vocab_size, 1);

    double** alpha_dW1 = multiply_scalar(dW1, model->alpha, model->dimension, model->vocab_size);
    double** W1_diff_alpha_dW1 = subtract(model->W1, alpha_dW1, model->dimension, model->vocab_size);
    free2D(model->W1, model->dimension, model->vocab_size);
    model->W1 = W1_diff_alpha_dW1;

    double** alpha_dW2 = multiply_scalar(dW2, model->alpha, model->vocab_size, model->dimension);
    double** W2_diff_alpha_dW2 = subtract(model->W2, alpha_dW2, model->vocab_size, model->dimension);
    free2D(model->W2, model->vocab_size, model->dimension);
    model->W2 = W2_diff_alpha_dW2;

    double** alpha_db1 = multiply_scalar(db1, model->alpha, model->dimension, 1);
    double** b1_diff_alpha_db1 = subtract(model->b1, alpha_db1, model->dimension, 1);
    free2D(model->b1, model->dimension, 1);
    model->b1 = b1_diff_alpha_db1;

    double** alpha_db2 = multiply_scalar(db2, model->alpha, model->vocab_size, 1);
    double** b2_diff_alpha_db2 = subtract(model->b2, alpha_db2, model->vocab_size, 1);
    free2D(model->b2, model->vocab_size, 1);
    model->b2 = b2_diff_alpha_db2;

    free2D(W2T, model->dimension, model->vocab_size);
    free2D(yhat_diff_y, model->vocab_size, model->batch_size);
    free2D(W2T_mul_yhat_diff_y, model->dimension, model->batch_size);
    free2D(relu_W2T_yhat_diff_y, model->dimension, model->batch_size);
    free2D(OnesVector, model->batch_size, 1);

    free2D(XT, model->batch_size, model->vocab_size);
    free2D(dW1_partial_product, model->dimension, model->vocab_size);
    free2D(dW1, model->dimension, model->vocab_size);

    free2D(AT, model->batch_size, model->dimension);
    free2D(dW2_partial_product, model->vocab_size, model->dimension);
    free2D(dW2, model->vocab_size, model->dimension);

    free2D(db1_partial_product, model->dimension, 1);
    free2D(db1, model->dimension, 1);

    free2D(db2_partial_product, model->vocab_size, 1);
    free2D(db2, model->vocab_size, 1);

    free2D(alpha_dW1, model->dimension, model->vocab_size);
    free2D(alpha_dW2, model->vocab_size, model->dimension);
    free2D(alpha_db1, model->dimension, 1);
    free2D(alpha_db2, model->vocab_size, 1);

}

void gradientDescent(EMBEDDING* model, bool save)
{
    double loss;
    for(int i=0; i < model->epochs; i++)
    {
        forward_propagation(model);
        loss = cost(model);
        printf("Epoch: %d Loss: %lf\n", i+1, loss);
        back_propagation(model);
        if(save == true && i%100 == 0)
        {
            extractEmbeddings(model);
            saveModel(model, false);
        }
    }
    printf("Training completed.\n\n");
}

void free2D(double** M, int m, int n)
{
    if (M == NULL)
        return;

    for(int i=0;i<m;++i)
        free(M[i]);
    free(M);
}

void free2D_int(int** M, int m, int n)
{
    if (M == NULL)
        return;

    for(int i=0;i<m;++i)
        free(M[i]);
    free(M);
}

void destroyModel(EMBEDDING* model)
{
    printf("Initiating Model Deletion...\n");
    if(model == NULL)
        return;

    printf("Deleting corpi...\n");
    free(model->corpus);
    free(model->clean_corpus);
    free(model->vocab);
    

    printf("Deleting Weights & Biases...\n");
    free2D(model->W1, model->dimension, model->vocab_size);
    free2D(model->W2, model->vocab_size, model->dimension);
    free2D(model->b1, model->dimension, 1);
    free2D(model->b2, model->vocab_size, 1);
    free2D(model->A1, model->dimension, model->batch_size);
    free2D(model->A2, model->vocab_size, model->batch_size);

    printf("Deleting X & y...\n");
    free2D(model->X, model->vocab_size, model->batch_size);
    free2D(model->Y, model->vocab_size, model->batch_size);

    printf("Deleting HashTable...\n");
    for(int i=0; i<model->vocab_size; ++i)
    {
        free2D(model->hashtable[i]->wordvector, 1, model->dimension);
        free2D_int(model->hashtable[i]->onehotvector, 1, model->vocab_size);
        free(model->hashtable[i]->word);
        free(model->hashtable[i]);
    }
    free(model->hashtable);

    printf("Deleting Model...\n");
    free(model);
    printf("Model destroyed\n\n");
} 

double** relu(double** X, int m, int n)
{
    double** result = createZerosArray(m, n);
    for(int i = 0; i<m; i++)
    {
        for(int j=0; j<n; j++)
        {
            if (X[i][j]<=0.0)
                result[i][j] = 0.0;
            else
                result[i][j] = X[i][j];
        }
    }
    return result;
}

double** softmax(double** M, int m, int n, int axis)
{
    double** softmax_out = createZerosArray(m, n);

    for(int i=0; i<m; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            softmax_out[i][j] = exp(M[i][j]);   
        }
    }

    double* colsums = (double*)malloc(n*sizeof(double));
    double* rowsums = (double*)malloc(m*sizeof(double));
    memset(rowsums, 0, m*sizeof(double));
    memset(colsums, 0, n*sizeof(double));
    double sum = 0.0;
    for(int i=0; i<m; ++i)
    {
        for(int j=0; j<n; ++j)
        {
            rowsums[i] += softmax_out[i][j];
            colsums[j] += softmax_out[i][j];
            sum += softmax_out[i][j];
        }
    }
    if(axis==0)  
    {
        for(int i=0; i<m; ++i)
        {
            for(int j=0; j<n; ++j)
            {
                softmax_out[i][j] /= colsums[j];
            }
        }
    }
    else if(axis==1) 
    {
        for(int i=0; i<m; ++i)
        {
            for(int j=0; j<n; ++j)
            {
                softmax_out[i][j] /= rowsums[i];
            }
        }
    }
    else 
    {
        for(int i=0; i<m; ++i)
        {
            for(int j=0; j<n; ++j)
            {
                softmax_out[i][j] /= sum;
            }
        }
    }
    free(colsums);
    free(rowsums);
    return softmax_out;
}

double** createArray(int m, int n, int random_state)
{
    srand(random_state);
    double** array = (double**)malloc(sizeof(double*)*m);
    for(int i = 0; i<m ;i++)
    {
        array[i] = (double*)malloc(sizeof(double)*n);
        for(int j = 0; j<n; j++)
            array[i][j] = ((double)rand()/(double)(RAND_MAX))/10;
    }
    return array;
}

double** createZerosArray(int m, int n)
{
    double** array = (double**)malloc(sizeof(double*)*m);
    for(int i = 0; i<m ;i++)
    {
        array[i] = (double*)malloc(sizeof(double)*n);
        for(int j = 0; j<n; j++)
            array[i][j] = 0;
    }
    return array;
}

double** createOnesArray(int m, int n)
{
    double** array = (double**)malloc(sizeof(double*)*m);
    for(int i = 0; i<m ;i++)
    {
        array[i] = (double*)malloc(sizeof(double)*n);
        for(int j = 0; j<n; j++)
            array[i][j] = 1;
    }
    return array;
}

double** multiply(double **M1, double **M2, int m1, int n1, int m2, int n2)
{
    double **result = createZerosArray(m1, n2);
    for(int i = 0; i<m1 ;i++)
    {
        for(int j=0; j<n2 ;j++)
        {
            for(int k=0; k<m2; k++)
                result[i][j]+= M1[i][k] * M2[k][j];
        }
    }
    return result;
}

double** add(double **M1, double **M2, int m, int n)
{
    double **result = createZerosArray(m, n);
    for(int i = 0; i<m; i++)
    {
        for(int j=0; j<n ;j++)
            result[i][j] = M1[i][j] + M2[i][j];
    }
    return result;
}

double** subtract(double **M1, double **M2, int m, int n)
{
    double **result = createZerosArray(m, n);
    for(int i = 0; i<m; i++)
    {
        for(int j=0; j<n ;j++)
            result[i][j] = M1[i][j] - M2[i][j];
    }
    return result;
}

double** multiply_scalar(double **M, double C, int m, int n)
{
    double **result = createZerosArray(m, n);
    for(int i = 0; i<m; i++)
    {
        for(int j=0; j<n ;j++)
            result[i][j] = M[i][j]*C;
    }
    return result;
}

double **transpose(double**A, int m, int n)
{
    double **trans = createZerosArray(n, m);
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            trans[i][j] = A[j][i];
    return trans;
}

int** createOneHot(NODE* node, EMBEDDING* model)
{
    int index = getHashvalue(node->word, model->vocab_size);
    while(model->hashtable[index] != NULL)
    {
        if(!strcmp(model->hashtable[index]->word, node->word))
            break;
        index = (index+1)%model->vocab_size;
    }
    int** oneHotVector = (int**)malloc(sizeof(int*));
    oneHotVector[0] = (int*)malloc(sizeof(int)*model->vocab_size);
    for(int i=0; i < model->vocab_size; i++)
        oneHotVector[0][i] = 0;
    oneHotVector[0][index] = 1;
    return oneHotVector;
}

double** broadcast_and_add(double** WX, double **b, int m1, int n1, int m2, int n2)
{
    double **Z1 = createZerosArray(m1, n1);
    for(int i=0; i<n1; i++)
    {
        for(int j=0; j<m1; j++)
            Z1[j][i] = WX[j][i] + b[j][0];
    }
    return Z1;
}

double dot(double** v1, double** v2, int n)
{
    double result = 0;
    for(int i = 0; i<n; i++)
        result+= v1[0][i] * v2[0][i];
    return result;
}

double cosine_similarity(double** v1, double** v2, int N)
{
    return dot(v1, v2, N)/(norm(v1, 1, N)*norm(v2, 1, N));
}

double cosine_distance(double** v1, double** v2, int N)
{
    return 1.0 - cosine_similarity(v2, v2, N);
}

double similarity(EMBEDDING* model, char* word1, char* word2)
{
    double** v1 = getVector(model, word1);
    double** v2 = getVector(model, word2);
    if(v1 == NULL)
    {
        printf("%s does not belong in vocabulary.\n", word1);
        return -1;
    }
    if(v2 == NULL)
    {
        printf("%s does not belong in vocabulary.\n", word2);
        return -1;
    }
    return cosine_similarity(v1, v2, model->dimension);
}

double distance(EMBEDDING* model, char* word1, char* word2)
{
    double** v1 = getVector(model, word1);
    double** v2 = getVector(model, word2);
    return 1.0-cosine_similarity(v1, v2, model->dimension);
}

double norm(double** M, int m, int n)
{
    double result = 0;
    for(int i = 0; i<m; i++)
    {
        for(int j = 0; j < n; j++)
            result+= M[i][j]*M[i][j];
    }
    result = sqrt(result);
    return result;
}

double** getX(EMBEDDING* model, int m, char* s)
{
    double** X = createZerosArray(model->vocab_size, m);
    char *token1, *save1, *token2, *save2;
    token1 = strtok_r(s, "\n", &save1);
    int col = 0;
    while(token1 != NULL && col <= m)
    {
        double** example = createZerosArray(1, model->vocab_size);
        int ctr = 0;
        token2 = strtok_r(token1, " ", &save2);
        while(token2 != NULL && ctr <= 2*model->context)
        {
            int index = getHashvalue(token2, model->vocab_size);
            while(model->hashtable[index] != NULL)
            {
                if(!strcmp(model->hashtable[index]->word, token2))
                    break;
                index = (index+1)%model->vocab_size;
            }
            int** oneHotVector = model->hashtable[index]->onehotvector;
            for(int j=0; j<model->vocab_size; j++)
                example[0][j]+= oneHotVector[0][j];
            token2 = strtok_r(NULL, " ", &save2);
            ctr++;
        }
        for(int j=0; j<model->vocab_size; j++)
            example[0][j]/= 2*model->context;
        for(int j = 0; j<model->vocab_size; j++)
            X[j][col] = example[0][j];
        col++;
        free2D(example, 1, model->vocab_size);
        token1 = strtok_r(NULL, "\n", &save1);
    }
    return X;
}

double** getY(EMBEDDING* model, int m, char* s)
{
    double** y = createZerosArray(model->vocab_size, m);
    char *token, *save; 
    token = strtok_r(s, "\n", &save);
    int col = 0;
    while(token != NULL && col <= m)
    {
        int index = getHashvalue(token, model->vocab_size);
        while(model->hashtable[index] != NULL)
        {
            if(!strcmp(model->hashtable[index]->word, token))
                break;
            index = (index+1)%model->vocab_size;
        }
        int** oneHotVector = model->hashtable[index]->onehotvector;
        for(int j = 0; j<model->vocab_size; j++)
            y[j][col] = oneHotVector[0][j];
        col++;
        token = strtok_r(NULL, "\n", &save);
    }
    return y;
}

char* remove_punctuations(char *sent)
{
	char punc[] = {'\n', '!', ']', '{', '#', '.', '<', '/', '(', '~', ',', '%', ';', '`', ':', '?', '+', '$', '^', '\\', '@', '*', '}', '=', '_', '\"', ')', '-', '|', '[', '&', '>'};
	int check, ctr = 0;
	char *s = (char*)malloc(sizeof(char)*strlen(sent));
	for (int i = 0; i< strlen(sent); i++)
	{
		check = 0;
		for (int j = 0; j< 31; j++)
		{
			if ((char)sent[i] == (char)punc[j])
			{
				check = 1;
				break;
			}
		}
		if (check==0)
			s[ctr] = sent[i];
        else
            s[ctr] = ' ';	
        ctr++;
	}
	s[ctr]='\0';	
	return s;
}

char* trim(char* word)
{
    int len = strlen(word);
    char* s = (char*)malloc(sizeof(char)*len);
    int pos1 = 0, pos2 = len-1, ctr = 0;
    while(word[pos1]==' ' || word[pos1]=='\t' || word[pos1]=='\n')
        pos1++;
    while(word[pos2]==' '|| word[pos2]=='\t' || word[pos2]=='\n')
        pos2--;
    while(pos1<=pos2)
    {
        s[ctr] = word[pos1];
        pos1++;
        ctr++;
    }
    s[pos1] = '\0';
    return s;
}

int getVocabularySize(EMBEDDING* model)
{
    char* corpus = model->clean_corpus;
    int len = strlen(corpus);
    char* temp1 = (char*)malloc(sizeof(char)*len);
    char* temp2 = (char*)malloc(sizeof(char)*len);
    char* temp3 = (char*)malloc(sizeof(char)*len);
    strcpy(temp1, corpus);
    strcpy(temp2, "");
    strcpy(temp3, "");
    char* token1;
    char* token2;
    char* save1;
    char* save2;
    int V = 0;
    
    token1 = strtok_r(temp1, " ", &save1);
    while(token1 != NULL)
    {
        strcpy(temp3, "");
        strcpy(temp3, temp2);        
        token2 = strtok_r(temp3, " ", &save2);
        bool flag = false;
        while(token2!=NULL)
        {
            if (!strcmp(token1, token2))
                flag = true;
            token2 = strtok_r(NULL, " ", &save2);
        }
        if (!flag)
        {
            strcat(temp2, token1);
            strcat(temp2, " ");
            V++;
        }
        token1 = strtok_r(NULL, " ", &save1);
    }
    int len2 = strlen(temp2);
    temp2[len2-1] = '\0';
    model->vocab = (char*)malloc(sizeof(char)*len2);
    strcpy(model->vocab, temp2);
    free(temp1);
    free(temp2);
    free(temp3);
    return V;
}