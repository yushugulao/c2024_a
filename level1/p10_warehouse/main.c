///created by whf

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_INVENTORY 100   // 最大存货数量
#define MAX_MODEL_LEN 50    // 型号最大长度
#define MAX_Q_NUM 100       // 输入最多位数

// 对存货结构进行定义
typedef struct {
    char model[MAX_MODEL_LEN];
    int quantity;
} InventoryItem;

void displayInventory(InventoryItem inventory[], int count);
void addStock(InventoryItem inventory[], int *count);
void removeStock(InventoryItem inventory[], int *count);
void clearStock(InventoryItem inventory[], int *count);
void checkWhetherNone(InventoryItem inventory[], int *count);
void loadInventoryFromFile(InventoryItem inventory[], int *count, const char *filename);
void saveInventoryToFile(InventoryItem inventory[], int count, const char *filename);

int main() {
    InventoryItem inventory[MAX_INVENTORY];
    int count = 0;  // 当前库存数量
    char filename[] = "inventory.txt";

    // 从文件中加载库存数据
    loadInventoryFromFile(inventory, &count, filename);

    int choice;
    char strchoice[MAX_Q_NUM];
    int strchoiceIndex=0;
    int inputc;

    do {

        choice=0;
        printf("\n help:\n");
        printf("1. the stock list\n");
        printf("2. add\n");
        printf("3. remove\n");
        printf("4. clear the stock\n");
        printf("5. exit\n");
        printf("please enter your choice(in number): ");
        backForth:strchoiceIndex=0;

        while (1) {
            inputc = getchar();
            if (inputc == '\n') {
                break;
            }
            if (inputc >= '0' && inputc <= '9') {
                inputc -= '0';
                strchoice[strchoiceIndex] = inputc;
                strchoiceIndex++;
            }
            else {
                goto backForth;
            }
        }
        for (int i = 0; i < strchoiceIndex; i++) {
            choice=choice*10+strchoice[i];
        }
        switch (choice) {
            case 1:
                displayInventory(inventory, count);
                break;
            case 2:
                addStock(inventory, &count);
                break;
            case 3:
                removeStock(inventory, &count);
                break;
            case 4:
                clearStock(inventory,&count);
                break;
            case 5:
                checkWhetherNone(inventory,&count);
                saveInventoryToFile(inventory, count, filename);
                printf("inventory saved,press any button to exit.\n");
                break;

            default:
                printf("I can't undersatand you , please try again.\n");

        }
    }
    while (choice != 5);
    return 0;
}

// 显示存货列表
void displayInventory(InventoryItem inventory[], int count) {
    printf("\nStocks in the warehouse:\n");
    if (count == 0) {
        printf("the warehouse is empty.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("model: %s, quantity: %d\n", inventory[i].model, inventory[i].quantity);
        }
    }
}

// 入库功能
void addStock(InventoryItem inventory[], int *count) {
    char model[MAX_MODEL_LEN];
    int quantity;

    printf("please enter the model:\n ");
    scanf("%s", model);
    printf("please enter the quantity:\n ");

    char strquantity[MAX_Q_NUM];
    int strquantityIndex=0;
    int inputq;
    inputq=0;
    int times=0;
    backForth: scanf("\n");
    quantity=0;

    while (1) {
        inputq = getchar();
        if (inputq == '\n') {
            break;
        }
        if (inputq >= '0' && inputq <= '9') {
            inputq -= '0';
            strquantity[strquantityIndex] = inputq;
            strquantityIndex++;
        }
        else {
            times++;
            if (times == 1) {
                printf("Please enter a number:\n");
            }
            goto backForth;
        }
    }
    for (int i = 0; i < strquantityIndex; i++) {
        quantity=quantity*10+strquantity[i];
    }


    // 检查该型号是否已存在
    for (int i = 0; i < *count; i++) {
        if (strcmp(inventory[i].model, model) == 0) {
            inventory[i].quantity += quantity;
            printf("addStock completed\n");
            return;
        }
    }

    // 如果型号不存在，开一个新的
    if (*count < MAX_INVENTORY) {
        strcpy(inventory[*count].model, model);
        inventory[*count].quantity = quantity;
        (*count)++;
        printf("addStock completed.\n");
    } else {
        printf("the warehouse is full,please remove sth to add more.\n");
    }
}

// 出库功能
void removeStock(InventoryItem inventory[], int *count) {
    char model[MAX_MODEL_LEN];
    int quantity;

    printf("please enter the model:");
    scanf("%s", model);
    printf("please enter the quantity: ");
    //scanf("%d", &quantity);
    char strquantity[MAX_Q_NUM];
    int strquantityIndex=0;
    int inputq;
    inputq=0;
    int times=0;
    backForth: scanf("\n");
    quantity=0;

    while (1) {
        inputq = getchar();
        if (inputq == '\n') {
            break;
        }
        if (inputq >= '0' && inputq <= '9') {
            inputq -= '0';
            strquantity[strquantityIndex] = inputq;
            strquantityIndex++;
        }
        else {
            times++;
            if (times == 1) {
                printf("Please enter a number:\n");
            }
            goto backForth;
        }
    }
    for (int i = 0; i < strquantityIndex; i++) {
        quantity=quantity*10+strquantity[i];
    }

    for (int i = 0; i < *count; i++) {
        if (strcmp(inventory[i].model, model) == 0) {
            if (inventory[i].quantity >= quantity) {
                inventory[i].quantity -= quantity;
                printf("removeStock completed.\n");
                checkWhetherNone(inventory,count);
            }
            else {
                printf("No enough stock of this kind in warehouse!\n");
            }
            return;
        }
    }
    printf("stock unfound.\n");
}

//清空库存
void clearStock(InventoryItem inventory[], int *count) {
    for (int i = 0; i < *count; i++) {
        inventory[i].quantity = 0;
    }
    checkWhetherNone(inventory,count);
}

//检测为空的型号将相关信息删除
void checkWhetherNone(InventoryItem inventory[], int *count) {
    int i=0;
    while(i < *count) {
        if (inventory[i].quantity == 0) {
            if (i==0) {
                (*count)--;
            }
            for (int j = i; j < *count-1; j++) {
                inventory[j] = inventory[j+1];
            }
            (*count)--;
            i--;
        }
        i++;
    }
}

// 从文件加载库存
void loadInventoryFromFile(InventoryItem inventory[], int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("file unfound,please create one.\n");
        return;
    }
    *count = 0;
    while (fscanf(file, "%s %d", inventory[*count].model, &inventory[*count].quantity) != EOF) {
        (*count)++;
    }
    fclose(file);
}

// 保存库存到文件
void saveInventoryToFile(InventoryItem inventory[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("The file unfound,please create a file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", inventory[i].model, inventory[i].quantity);
    }

    fclose(file);
}
