# cfbdiff - Control Flow Based diff
Create a control flow based diff from the outputs of 'g++ -fdump-tree-cfg.'
Currently, supported files are C source code only.

There is no warranty for the outputs.

# Requirement
g++ 9.4.0

The compiler option -fdump-tree-cfg=_filename_ can use g++ 4.8.0 or later,
but the format may differ by version.
So there is no warranty for other versions.

# Example
bef.c:
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
    int type;

    type = rand() % 4;

    if (type == 1) {
        printf("hoge()");
        printf("fuga()");
    } else {
        printf("piyo()");
        printf("poyo()");
    }

    printf("foo()");
    printf("bar()");

    return 0;
}
```
aft.c
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
    int type;

    type = rand() % 4;

    switch (type) {
        case 1:
            printf("hoge()");
            // oops
            break;
        case 2:
            printf("newHoge()");
            break;
        default:
            printf("piyo()");
            printf("poyo()");
            break;
    }

    printf("foo()");
    printf("bar()");

    return 0;
}
```

command:
```shell
$ ./cfbdiff bef.c aft.c
Function: "main" block deleted:
  printf ("hoge()");
  printf ("fuga()");


Function: "main" block added:
  printf ("newHoge()");


Function: "main" block added:
  printf ("hoge()");


```
