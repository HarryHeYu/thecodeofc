#include <stdio.h>
#include <string.h>

//如果在 Windows 系统，包含 windows.h 并设置控制台编码
#ifdef _WIN32
#include <windows.h>
#endif

int DFS(int currentStep, int totalSteps, int isLeftFoot){
    if(currentStep > totalSteps) return 0;
    if (currentStep == totalSteps) {
        return isLeftFoot == 1; // 确保最后一步是右脚
    }

    int ways = 0;
    if(isLeftFoot){
        ways += DFS(currentStep + 1, totalSteps, 0);
        ways += DFS(currentStep + 2, totalSteps, 0);
    } else {
        ways += DFS(currentStep + 1, totalSteps, 1);
        ways += DFS(currentStep + 2, totalSteps, 1);
    }
    return ways;
}

int main(){
    // 如果是 Windows 系统，设置输出编码为 UTF-8
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    int totalSteps = 39;
    int result = DFS(0, totalSteps, 1);
    printf("总共有%d种不同的上法。\n", result);
    return 0;
}
int ans;
void dfs(int step, int n){
    if(n > 39)return;
    if(step % 2 == 0 && n == 39){
        ans ++;
        return;
    }
    dfs(step + 1, n + 1);
    dfs(step + 1, n + 2);
}
int main(){
    dfs(0,0);
    printf("%d",ans);
    return 0;
}