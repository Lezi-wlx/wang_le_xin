import easygui as eg
import os

# 数据保存文件：投票记录（含态度标记）、已投票学号
VOTE_FILE = "vote_data.txt"    # 格式：学号|喜欢:选项1,选项2|不喜欢:选项1,选项2
STUDENT_FILE = "student_ids.txt"# 格式：一行一个已投票学号
# 统一活动选项列表
ACTIVITIES = ['听音乐', '看边画边说', '看小猪佩奇', '其他']

def load_existing_students():
    """加载已投票学号，返回集合用于快速校验"""
    students = set()
    if os.path.exists(STUDENT_FILE):
        try:
            with open(STUDENT_FILE, 'r', encoding='utf-8') as f:
                for line in f.readlines():
                    sid = line.strip()
                    if sid:
                        students.add(sid)
        except Exception as e:
            eg.msgbox(f"加载学号失败: {str(e)}", "错误")
    return students

def load_vote_data():
    """加载投票数据，返回统计字典：{态度: {活动: 票数}}"""
    stats = {
        "喜欢": {act: 0 for act in ACTIVITIES},
        "不喜欢": {act: 0 for act in ACTIVITIES}
    }
    if os.path.exists(VOTE_FILE):
        try:
            with open(VOTE_FILE, 'r', encoding='utf-8') as f:
                for line in f.readlines():
                    line = line.strip()
                    if "|" not in line:
                        continue
                    # 拆分学号、喜欢列表、不喜欢列表
                    _, like_part, dislike_part = line.split("|", 2)
                    # 统计“喜欢”票数
                    like_acts = like_part.replace("喜欢:", "").split(",") if like_part else []
                    for act in like_acts:
                        if act in stats["喜欢"]:
                            stats["喜欢"][act] += 1
                    # 统计“不喜欢”票数
                    dislike_acts = dislike_part.replace("不喜欢:", "").split(",") if dislike_part else []
                    for act in dislike_acts:
                        if act in stats["不喜欢"]:
                            stats["不喜欢"][act] += 1
        except Exception as e:
            eg.msgbox(f"加载投票数据失败: {str(e)}", "错误")
    return stats

def save_vote(student_id, like_acts, dislike_acts):
    """保存投票记录（含态度标记）和学号"""
    try:
        # 处理选项格式：空列表转空字符串，避免多余逗号
        like_str = "喜欢:" + ",".join(like_acts) if like_acts else "喜欢:"
        dislike_str = "不喜欢:" + ",".join(dislike_acts) if dislike_acts else "不喜欢:"
        # 1. 保存投票记录
        with open(VOTE_FILE, 'a', encoding='utf-8') as f:
            f.write(f"{student_id}|{like_str}|{dislike_str}\n")
        # 2. 保存已投票学号
        with open(STUDENT_FILE, 'a', encoding='utf-8') as f:
            f.write(f"{student_id}\n")
        return True
    except Exception as e:
        eg.msgbox(f"保存投票失败: {str(e)}", "错误")
        return False

def show_statistics():
    """显示“喜欢”和“不喜欢”的双向统计结果（管理员功能）"""
    stats = load_vote_data()
    total_voters = len(load_existing_students())
    # 拼接统计信息
    msg = f"中午活动投票统计\n"
    msg += f"总投票人数：{total_voters}人\n\n"
    # 先展示“喜欢”统计
    msg += "【喜欢的活动】\n"
    for act, count in stats["喜欢"].items():
        msg += f"{act}：{count}票\n"
    # 再展示“不喜欢”统计
    msg += "\n【不喜欢的活动】\n"
    for act, count in stats["不喜欢"].items():
        msg += f"{act}：{count}票\n"
    eg.msgbox(msg, "统计结果")

def main():
    program_title = "统计"
    # 1. 学号校验（防止重复投票）
    student_id = eg.enterbox("请输入你的学号：", program_title)
    if student_id is None:
        return
    student_id = student_id.strip()
    if not student_id:
        eg.msgbox("学号不能为空！", "提示")
        return
    # 检查是否已投票
    if student_id in load_existing_students():
        eg.msgbox(f"学号 {student_id} 已投过票，不能重复提交！", "提示")
        return
    # 2. 管理员查看统计
    if student_id == "adminstraror":
        show_statistics()
        return
    # 3. 投票选择（区分喜欢/不喜欢）
    # 第一个问题：喜欢的活动
    like_acts = eg.multchoicebox(
        "你们喜欢中午干什么？（可多选）",
        program_title,
        ACTIVITIES
    ) or []  # 取消时设为空列表
    # 第二个问题：不喜欢的活动
    dislike_acts = eg.multchoicebox(
        "你们不喜欢中午干什么？（可多选）",
        program_title,
        ACTIVITIES
    ) or []
    # 避免矛盾选择：同一活动不能既喜欢又不喜欢
    conflict_acts = set(like_acts) & set(dislike_acts)
    if conflict_acts:
        conflict_str = "、".join(conflict_acts)
        eg.msgbox(f"选择冲突：{conflict_str} 不能同时在「喜欢」和「不喜欢」列表中，请重新选择！", "提示")
        return
    # 4. 保存并提示成功
    if save_vote(student_id, like_acts, dislike_acts):
        eg.msgbox("投票已提交，感谢参与！", "成功")

if __name__ == "__main__":
    main()
