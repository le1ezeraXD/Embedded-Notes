# Linux 常用命令
> 整理日期: 06/11/2026

## 1. 包管理 (apt)
`apt` 是 Ubuntu 的包管理器,类似 Python 的 `pip`,一条命令从仓库下载安装并处理依赖。

```bash
sudo apt update              # 更新"软件清单"(看看仓库里有什么新的),装软件前常先跑这个
sudo apt install 包名         # 安装软件
sudo apt remove 包名          # 卸载软件
sudo apt upgrade             # 升级所有已安装的软件到最新版
apt search 关键词             # 搜索有没有某个包(不用sudo)
apt show 包名                 # 看某个包的详细信息
```

## 2. 文件操作
```bash
ls              # 列出当前目录的文件(ls -l 详细信息,ls -a 含隐藏文件)
cd 目录          # 进入某目录(cd .. 返回上级, cd ~ 回家目录)
pwd             # 显示当前在哪个目录(print working directory)
mkdir 目录名     # 新建目录
rm 文件          # 删除文件(rm -r 目录 删除目录, 慎用!Linux删了不进回收站)
cp 源 目标       # 复制
mv 源 目标       # 移动/重命名
cat 文件         # 显示文件内容
```
## 3. 查看/编辑文件
```bash
cat 文件         # 显示整个文件(短文件)
less 文件        # 分页查看(长文件,按q退出)
head 文件        # 看前几行
tail 文件        # 看后几行(tail -f 实时跟踪日志,调试常用!)
vim 文件         # 用vim编辑(嵌入式开发标配编辑器,值得学)
nano 文件        # 更简单的编辑器(新手友好)
```
## 4. 权限相关
```bash
sudo 命令        # 以管理员权限执行
chmod +x 文件    # 给文件加"可执行"权限(运行脚本前常用)
chmod 755 文件   # 设置权限(数字权限,以后会学)
```
## 5. 编译构建
```bash
cmake ..         # 生成编译配置
make             # 编译(make -j4 用4线程加速)
make clean       # 清理编译产物
```
## 6. 进程/系统
```bash
ps              # 看进程
top / htop      # 看系统资源占用(实时)
kill 进程号      # 结束进程
df -h           # 看磁盘空间
```
## 7.搜索
```bash
grep "关键词" 文件     # 在文件里搜索某个词(调试看log必备!)
find 路径 -name "*.c" # 找文件
which 命令            # 看某个命令在哪(比如 which python)
```
## 8. 远程操作(搭环境用到的)
```bash
scp -r 用户@IP:远程路径 .    # 从服务器拷文件到当前目录
ssh 用户@IP                 # 远程登录服务器
```

---