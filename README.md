# NLPC
Natural Language Toolkit built using the C Programming Language.

# Running NLPC
You can use NLPC simply by including the main header file:

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlpc.h"
int main(){
    ...
    ...
}
```
Alternatively, you can include only the utilities desired

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stemmer.h"
#include "preprocess.h"
int main(){
    ...
    ...
}
```

# To-Do
*   Change struct names in w2v and lemmatizer
*   Testing
*   Code cleanup - memset, free() and so on