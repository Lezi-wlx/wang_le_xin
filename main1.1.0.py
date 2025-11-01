import tkinter as tk
from tkinter import messagebox
import easygui as eg
import os
import pyperclip
import time
from datetime import datetime

class MusicPlayer:
    def __init__(self, root):
        self.root = root
        self.root.title("Musicing")
        self.root.geometry("350x500")  # 增大窗口以容纳新内容
        self.root.resizable(True, True)
        
        # 歌曲列表和选项设置
        self.song_list = []
        self.win7_compatibility = True
        self.encoding = "utf-8"
        
        # 加载数据
        self.load_songs()
        self.load_options()
        
        # 创建UI
        self.create_widgets()
    
    def create_widgets(self):
        # 标题文本
        title_frame = tk.Frame(self.root)
        title_frame.pack(pady=10)
        
        title_label = tk.Label(title_frame, text="Musicing Version 1.0.6\nPython Edition\n找乐子工作室 请勿二次分发！", 
                              justify=tk.CENTER, font=("SimHei", 10))
        title_label.pack()
        
        # 1. 实时时间显示
        time_frame = tk.Frame(self.root)
        time_frame.pack(pady=5)
        
        self.time_label = tk.Label(time_frame, text="", font=("SimHei", 10))
        self.time_label.pack()
        self.update_time()  # 启动时间更新
        
        # 2. 表格显示
        table_frame = tk.Frame(self.root)
        table_frame.pack(pady=10, fill=tk.X, padx=20)
        
        # 表格标题行
        header_frame = tk.Frame(table_frame)
        header_frame.pack(fill=tk.X)
        tk.Label(header_frame, text="------------------------------------------------", width=60).pack(anchor=tk.W)
        
        # 星期行
        week_frame = tk.Frame(table_frame)
        week_frame.pack(fill=tk.X)
        tk.Label(week_frame, text="周一|周二|周三|周四|周五", width=60, justify=tk.LEFT).pack(anchor=tk.W)
        
        # 内容行
        content_frame = tk.Frame(table_frame)
        content_frame.pack(fill=tk.X)
        tk.Label(content_frame, text="------------------------------------------------", width=60).pack(anchor=tk.W)
        tk.Label(content_frame, text="小猪佩奇|音乐|小猪佩奇|音乐|混子哥边画边说", width=60, justify=tk.LEFT).pack(anchor=tk.W)
        tk.Label(table_frame, text="------------------------------------------------", width=60).pack(anchor=tk.W)
        
        # 按钮区域
        button_frame = tk.Frame(self.root)
        button_frame.pack(pady=10, fill=tk.BOTH, expand=True)
        
        buttons = [
            ("查看所有歌曲", self.view_all_songs),
            ("播放下一首歌曲", self.play_next_song),
            ("添加新歌曲", self.add_new_song),
            ("删除已存在的歌曲", self.delete_song),
            ("查找已存在的歌曲", self.find_song),
            ("替换已存在的歌曲", self.replace_song),
            ("选项", self.open_options),
            ("关闭", self.root.quit)
        ]
        
        # 网格布局放置按钮
        for i, (text, command) in enumerate(buttons):
            btn = tk.Button(button_frame, text=text, command=command, width=20, height=2)
            btn.grid(row=i//2, column=i%2, padx=10, pady=5)
    
    def update_time(self):
        """实时更新时间显示"""
        # 格式：2025年11月1日 17点59分59秒 星期六
        now = datetime.now()
        weekday_map = ["星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"]
        weekday = weekday_map[now.weekday()]
        time_str = now.strftime(f"%Y年%m月%d日 %H点%M分%S秒 {weekday}")
        self.time_label.config(text=time_str)
        self.root.after(1000, self.update_time)  # 每秒更新一次
    
    def load_songs(self):
        """从文件加载歌曲列表"""
        if os.path.exists("music.txt"):
            try:
                with open("music.txt", "r", encoding=self.encoding) as f:
                    self.song_list = [line.strip() for line in f.readlines() if line.strip()]
            except Exception as e:
                eg.msgbox(f"加载歌曲列表失败: {str(e)}", "错误")
    
    def save_songs(self):
        """保存歌曲列表到文件"""
        try:
            with open("music.txt", "w", encoding=self.encoding) as f:
                for song in self.song_list:
                    f.write(song + "\n")
        except Exception as e:
            eg.msgbox(f"保存歌曲列表失败: {str(e)}", "错误")
    
    def load_options(self):
        """从文件加载选项设置"""
        if os.path.exists("options.txt"):
            try:
                with open("options.txt", "r", encoding="utf-8") as f:
                    for line in f.readlines():
                        line = line.strip()
                        if line.startswith("win7_compatibility:"):
                            self.win7_compatibility = line.split(":")[1].strip().lower() == "true"
                        elif line.startswith("encoding:"):
                            self.encoding = line.split(":")[1].strip()
                            # 确保编码是有效的选项
                            if self.encoding not in ["ansi", "unicode", "utf-8"]:
                                self.encoding = "utf-8"
            except Exception as e:
                eg.msgbox(f"加载选项失败: {str(e)}", "错误")
    
    def save_options(self):
        """保存选项设置到文件"""
        try:
            with open("options.txt", "w", encoding="utf-8") as f:
                f.write(f"win7_compatibility: {str(self.win7_compatibility).lower()}\n")
                f.write(f"encoding: {self.encoding}\n")
        except Exception as e:
            eg.msgbox(f"保存选项失败: {str(e)}", "错误")
    
    def view_all_songs(self):
        """查看所有歌曲"""
        if not self.song_list:
            eg.msgbox("当前歌单为空", "查看所有歌曲")
            return
            
        content = "当前歌单：\n" + "\n".join(self.song_list)
        eg.msgbox(content, "查看所有歌曲")
    
    def play_next_song(self):
        """播放下一首歌曲"""
        if not self.song_list:
            eg.msgbox("歌单为空，没有可播放的歌曲", "播放下一首歌曲")
            return
            
        next_song = self.song_list.pop(0)
        self.save_songs()  # 保存更新后的列表
        pyperclip.copy(next_song)  # 复制到剪贴板
        eg.msgbox(f"下一首是：\n{next_song}\n已将歌曲复制到剪贴板\n已删除该歌曲", "播放下一首歌曲")
    
    def add_new_song(self):
        """添加新歌曲"""
        song_name = eg.enterbox("请输入新歌曲名：", "添加新歌曲")
        if song_name is None:  # 用户点击了cancel
            return
            
        song_name = song_name.strip()
        if not song_name:
            eg.msgbox("歌曲名不能为空", "错误")
            return
            
        if song_name in self.song_list:
            eg.msgbox(f"歌曲 '{song_name}' 已存在", "提示")
            return
            
        self.song_list.append(song_name)
        self.save_songs()
        eg.msgbox(f"已添加新歌曲：{song_name}", "添加新歌曲")
    
    def delete_song(self):
        """删除已存在的歌曲"""
        song_name = eg.enterbox("请输入要删除的歌曲名：", "删除已存在的歌曲")
        if song_name is None:  # 用户点击了cancel
            return
            
        song_name = song_name.strip()
        if not song_name:
            eg.msgbox("歌曲名不能为空", "错误")
            return
            
        if song_name in self.song_list:
            self.song_list.remove(song_name)
            self.save_songs()
            eg.msgbox(f"已删除歌曲：{song_name}", "删除已存在的歌曲")
        else:
            eg.msgbox(f"未找到歌曲：{song_name}", "删除已存在的歌曲")
    
    def find_song(self):
        """查找已存在的歌曲"""
        song_name = eg.enterbox("请输入要查找的歌曲名：", "查找已存在的歌曲")
        if song_name is None:  # 用户点击了cancel
            return
            
        song_name = song_name.strip()
        if not song_name:
            eg.msgbox("歌曲名不能为空", "错误")
            return
            
        if song_name in self.song_list:
            index = self.song_list.index(song_name) + 1  # 位置从1开始
            eg.msgbox(f"已查找到歌曲：{song_name}\n位置：{index}", "查找已存在的歌曲")
        else:
            eg.msgbox(f"未查找到歌曲：{song_name}", "查找已存在的歌曲")
    
    def replace_song(self):
        """替换已存在的歌曲"""
        # 获取要被替换的歌曲
        old_song = eg.enterbox("请输入要被替换掉的歌曲名（已存在）：", "替换已存在的歌曲")
        if old_song is None:  # 用户点击了cancel
            return
            
        old_song = old_song.strip()
        if not old_song:
            eg.msgbox("歌曲名不能为空", "错误")
            return
            
        if old_song not in self.song_list:
            eg.msgbox(f"未查找到歌曲：{old_song}", "替换已存在的歌曲")
            return
            
        # 获取要替换成的歌曲
        new_song = eg.enterbox("请输入要替换成的歌曲名（未存在）：", "替换已存在的歌曲")
        if new_song is None:  # 用户点击了cancel
            return
            
        new_song = new_song.strip()
        if not new_song:
            eg.msgbox("歌曲名不能为空", "错误")
            return
            
        if new_song in self.song_list:
            eg.msgbox(f"歌曲 '{new_song}' 已存在", "提示")
            return
            
        # 执行替换
        index = self.song_list.index(old_song)
        self.song_list[index] = new_song
        self.save_songs()
        eg.msgbox(f"已替换歌曲：{old_song} → {new_song}\n位置：{index + 1}", "替换已存在的歌曲")
    
    def open_options(self):
        """打开选项窗口"""
        options_window = tk.Toplevel(self.root)
        options_window.title("选项")
        options_window.geometry("300x200")
        options_window.resizable(False, False)
        options_window.transient(self.root)  # 设置为主窗口的子窗口
        options_window.grab_set()  # 模态窗口
        
        # Windows 7兼容性模式
        win7_var = tk.BooleanVar(value=self.win7_compatibility)
        win7_check = tk.Checkbutton(options_window, text="Windows7兼容性模式", variable=win7_var)
        win7_check.pack(anchor=tk.W, padx=20, pady=5)
        
        # 编码格式选择
        tk.Label(options_window, text="记事本编码格式：").pack(anchor=tk.W, padx=20, pady=5)
        
        encoding_var = tk.StringVar(value=self.encoding)
        
        ansi_radio = tk.Radiobutton(options_window, text="ansi", variable=encoding_var, value="ansi")
        ansi_radio.pack(anchor=tk.W, padx=40)
        
        unicode_radio = tk.Radiobutton(options_window, text="unicode", variable=encoding_var, value="unicode")
        unicode_radio.pack(anchor=tk.W, padx=40)
        
        utf8_radio = tk.Radiobutton(options_window, text="utf-8", variable=encoding_var, value="utf-8")
        utf8_radio.pack(anchor=tk.W, padx=40)
        
        # 保存按钮
        def save_options():
            self.win7_compatibility = win7_var.get()
            self.encoding = encoding_var.get()
            self.save_options()
            # 重新加载歌曲列表（使用新的编码）
            self.load_songs()
            options_window.destroy()
        
        save_btn = tk.Button(options_window, text="保存", command=save_options)
        save_btn.pack(pady=10)


if __name__ == "__main__":
    root = tk.Tk()
    app = MusicPlayer(root)
    root.mainloop()
