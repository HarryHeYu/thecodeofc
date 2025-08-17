#include <stdio.h>
int main() {
    int n;
    scanf("%d", &n);
    int next[n + 1];  // 用来存储每个小朋友后面的小朋友编号，1-based索引
    for (int i = 1; i <= n; i++) {
        scanf("%d", &next[i]);
    }
    int h;
    scanf("%d", &h);
    // 从队首开始，依次输出小朋友编号
    int current = h;
    while (current != 0) {
        printf("%d ", current);
        current = next[current];
    }
    return 0;
}
