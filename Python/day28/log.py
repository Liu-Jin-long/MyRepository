# 编写于2022年06月30日19时10分27秒
# %(levelno)s: 打印日志级别的数值
# %(levelname)s: 打印日志级别名称
# %(pathname)s: 打印当前执行程序的路径,其实就是sys.argv[0]
# %(filename)s: 打印当前执行程序名
# %(funcName)s: 打印日志的当前函数
# %(lineno)d: 打印日志的当前行号
# %(asctime)s: 打印日志的时间
# %(threadName)s: 打印线程名称
# %(thread)d: 打印线程ID
# %(process)d: 打印进程ID
# %(message)s: 打印日志信息


# import logging
# #对日志的输出格式及方式做相关配置  WARNING级别及以上的日志才会输出控制台
# logging.basicConfig(level=logging.WARNING,
#                     format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s:%(message)s')
# logging.info('这是logging info的message')
# logging.debug('这是logging debug的message')
# logging.warning('这是logging warning的message')
# logging.error('这是logging error的message')
# logging.critical('这是logging critical的message')


############################################################################

# import logging
#
# # 日志输出到文件
# logging.basicConfig(level=logging.WARNING, filename='log.txt', filemode='w', encoding='utf8',
#                     format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s:%(message)s')
# logging.info('这是logging info的message')
# logging.debug('这是logging debug的message')
# logging.warning('这是logging warning的message')
# logging.error('这是logging error的message')
# logging.critical('这是logging critical的message')

#######################################################################

import logging

# 把日志输出到控制台，同时写入文件
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)  # 日志等级总开关
log_file = './log.txt'
file_handler = logging.FileHandler(log_file, mode='a', encoding='utf8')
file_handler.setLevel(logging.INFO)  # 输出到文件的日志等级开关，会受到总开关约束
console_handler = logging.StreamHandler()
console_handler.setLevel(logging.WARNING)  # 输出到控制台的日志等级开关，会受到总开关约束
formatter = logging.Formatter('%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s:%(message)s')
file_handler.setFormatter(formatter)
console_handler.setFormatter(formatter)

logger.addHandler(file_handler)
logger.addHandler(console_handler)

logging.info('这是logging info的message')
logging.debug('这是logging debug的message')
logging.warning('这是logging warning的message')
logging.error('这是logging error的message')
logging.critical('这是logging critical的message')
