#include <iostream>
#include <list>
#include <algorithm>
#define MAX_ACTIVE 131072           // Active List 的最大頁面數量
#define MAX_INACTIVE 131072          // Inactive List 的最大頁面數量
#define MAX_RECENTLY_REMOVED 131072  // Recently Removed List 的最大頁面數量
int HitCnt, MissCnt;

class PageManager
{
private:
    std::list<int> activeList;
    std::list<int> inactiveList;
    std::list<int> recentlyRemoved;

public:
    // 添加頁面到指定列表
    void addToList(std::list<int> &list, int pageId)
    {
        list.push_front(pageId);
    }

    // 從列表中移除頁面
    bool removeFromList(std::list<int> &list, int pageId)
    {
        auto it = std::find(list.begin(), list.end(), pageId);
        if (it != list.end())
        {
            list.erase(it);
            return true;
        }
        return false;
    }

    // 檢查是否最近被移除
    bool isRecentlyRemoved(int pageId)
    {
        return std::find(recentlyRemoved.begin(), recentlyRemoved.end(), pageId) != recentlyRemoved.end();
    }

    // 處理頁面訪問
    void accessPage(int pageId)
    {
        // 檢查 Active List
        if (std::find(activeList.begin(), activeList.end(), pageId) != activeList.end())
        {
            // std::cout << "Page " << pageId << " is accessed and already in Active List.\n";
            HitCnt++;
            removeFromList(activeList, pageId);
            addToList(activeList, pageId);  //移到頭部
            return; // 頁面已在 Active List 中
        }

        // 檢查 Inactive List
        if (std::find(inactiveList.begin(), inactiveList.end(), pageId) != inactiveList.end())
        {
            // std::cout << "Page " << pageId << " is moved from Inactive to Active List.\n";
            HitCnt++;
            //removeFromList(inactiveList, pageId);
            //addToList(activeList, pageId);
            return; // 從 Inactive 移動到 Active
        }

        // 如果頁面既不在 Active 也不在 Inactive List
        if (isRecentlyRemoved(pageId))
        {
            // std::cout << "Page " << pageId << " was recently removed, adding back to Active List.\n";
            MissCnt++;
            addToList(activeList, pageId);
            removeFromList(recentlyRemoved, pageId);
        }
        else
        {
            // std::cout << "Page " << pageId << " is new, adding to Inactive List.\n";
            MissCnt++;
            addToList(inactiveList, pageId);
        }

        balance();
    }

    void balance()
    {
        // 確保 activeList 不超過最大容量
        while (activeList.size() > MAX_ACTIVE)
        {
            int pageId = activeList.back();
            activeList.pop_back();
            addToList(inactiveList, pageId);
        }

        // 確保 inactiveList 不超過最大容量
        while (inactiveList.size() > MAX_INACTIVE)
        {
            int pageId = inactiveList.back();
            inactiveList.pop_back();
            addToList(recentlyRemoved, pageId);
        }

        // 確保 recentlyRemoved 不超過最大容量
        while (recentlyRemoved.size() > MAX_RECENTLY_REMOVED)
        {
            recentlyRemoved.pop_back();
        }
    }

    // 列印列表
    void printList()
    {
        std::cout << "Active List:" << activeList.size() << "\n";
        for (int id : activeList)
        {
            std::cout << id << " ";
        }
        std::cout << "\nInactive List:" << inactiveList.size() << "\n";
        for (int id : inactiveList)
        {
            std::cout << id << " ";
        }
        std::cout << "\nRecentRemoved List:" << recentlyRemoved.size() << "\n";
        for (int id : recentlyRemoved)
        {
            std::cout << id << " ";
        }
        std::cout << "\n-----------------\n";
    }
};

int main(int argc, char *argv[])
{
    PageManager manager;
    HitCnt = 0;
    MissCnt = 0;


    // 檢查參數
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <log_file_name>\n", argv[0]);
        return -1;
    }

    // 開檔
    FILE *log_file = fopen(argv[1], "r");
    if (log_file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    // 讀檔 + 模擬頁面訪問
    char phy_addr_str[256];
    unsigned long long phy_addr = 0;
    int cnt = 0;
    while (fscanf(log_file, "%s", phy_addr_str) != EOF)
    {
        // 將讀取的16進制字符串轉換為整數
        sscanf(phy_addr_str, "%llx", &phy_addr);
        // 輸出轉換後的整數
        unsigned int phy_frame_num = phy_addr >> 12;
        // printf("%s %u\n", phy_addr_str, phy_frame_num);
        printf("%d\r\t\t\t\t\t\r", ++cnt);

        manager.accessPage(phy_frame_num);
    }

    std::cout << "\nHitCnt: " << HitCnt << "\nMissCnt: " << MissCnt;
    printf("Hit Ratio = %5.4f %%\n", ((float)(HitCnt * 100) / (HitCnt + MissCnt)));

    return 0;
}