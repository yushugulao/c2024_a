### 题目：奔跑的字母

### 功能要求：

1. 在console上输出一个字母（或单词）
1. 使该字母或单词，按一定速度从左至右运动
1. 当遇到屏幕边界时反向运动

### vss library no conflict
/// cvs/svn conflict and combination
/// DCVS distribute --> charge --> GIT

/// GIT 每个人的工作环境自成体系 
. git 文件为database的目录
通过有向图结构进行过去文件版本追溯
working folder 可显示可追溯的版本
在本地保存完整的信息 
要上传修改就传changelist 无需整体，只需差异
Github利用了git 的基本工作原理，为一个便于共享的服务器
repo 存储了.git文件，需要时可展开为工作目录
由此可以通过github进行先前的拷贝行为

gitclone把服务器的git文件下载，转入工作目录（git依然是一个change）
git add可以添加任何一种变化：增删改
git commit将变化转化为working folder 的有向图
git push 将提交正式发送给服务器
git pull可以将修改过的部分拉回本地
