#include <stdio.h>
#include <string.h>

char memory[65536];
char data[256 * 1024];

void print_text(const char *s) {
    while (*s) putchar(*s++);
}

void print_number(unsigned long long x) {
    char digits[32];
    int n = 0;
    do {
        digits[n++] = '0' + x % 10;
        x /= 10;
    } while (x);
    while (n) putchar(digits[--n]);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        setbuf(stdin, NULL);
    } else if (argc != 2 || strcmp(argv[1], "--buffered") != 0) {
        print_text("Usage: ./main [--buffered]\n");
        return 1;
    }
    unsigned n = 0;
    int c, count = 0;
    while ((c = getchar()) >= '0' && c <= '9') {
        n = n * 10 + c - '0';
        count++;
        if (n > 100000) {
            print_text("N is too large\n");
            return 1;
        }
    }
    if (!count || c != '\n') {
        print_text("Expected N on the first line\n");
        return 1;
    }
    char model[128];
    int len = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len >= 127) {
            print_text("Model name is too long\n");
            return 1;
        }
        model[len++] = (char)c;
    }
    model[len] = 0;
    if (!len) {
        print_text("Expected CPU model on the second line\n");
        return 1;
    }
    const char home_model[] = "i5-12450H";
    int same = 1;
    for (int i = 0; home_model[i] || model[i]; i++)
        if (home_model[i] != model[i]) same = 0;
    if (!same) {
        print_text("Wrong CPU model\n");
        return 1;
    }
    unsigned long long sum = 0;
    for (unsigned i = 1; i <= n; i++) {
        sum += i;
        memory[i % 65536] = (char)(i % 256);
        data[(i * 131u) % (256u * 1024u)] = (char)((sum + i) % 256);
    }
    print_text("Sum: ");
    print_number(sum);
    putchar('\n');
    char digits[32];
    int k = 0;
    do {
        digits[k++] = '0' + sum % 10;
        sum /= 10;
    } while (sum);
    print_text("Code: ");
    for (int i = k - 1, j = 0; i >= 0; i--, j++)
        putchar('0' + (digits[i] - '0' + (unsigned char)model[j % len]) % 10);
    putchar('\n');
    return 0;
}
