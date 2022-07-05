# 编写于2022年06月05日18时20分38秒
def BCMS_menu():
    """显示【名片管理系统】菜单选项"""
    print('*' * 20)
    print()
    print('1、新建名片\n2、显示全部\n3、查询名片\n\n0、退出BCMS\n')
    print('*' * 20)


def create_card():
    '''1、新建名片'''
    print('-' * 20)
    print('开始新建名片，请按照要求输入信息！')
    name = input('请输入姓名：')
    phone_num = input('请输入电话号码：')
    qq_num = input('请输入QQ号码：')
    e_mail = input('请输入电子邮箱：')
    new_card_dict = {'name': name, 'phone_num': phone_num, 'qq_num': qq_num, 'e_mail': e_mail}
    card_list.append(new_card_dict)
    print('已添加名片信息：', end='')
    # print('成功添加%s的名片' % new_card_dict['name'])
    print(f'成功添加{new_card_dict["name"]}的名片')


def show_info():
    '''2、显示全部'''
    print('-' * 20)
    print('显示已有的全部名片信息：')
    if len(card_list) == 0:
        print('提示：目前还没有任何名片记录！')
        return
    for name in ['姓名', '电话号码', 'QQ号码', '电子邮箱']:
        print(name, end='\t\t')
    print()
    for card in card_list:
        print('%s\t\t%s\t\t%s\t\t%s' % (card['name'], card['phone_num'], card['qq_num'], card['e_mail']))


def search_card():
    '''3、查询名片'''
    print('-' * 20)
    print('搜索已有的名片信息：')
    find_name = input('请输入要查询的姓名：')
    for card in card_list:
        if card['name'] == find_name:
            print(f'{find_name}的名片信息如下：')
            for name in ['姓名', '电话号码', 'QQ号码', '电子邮箱']:
                print(name, end='\t\t')
            print('\n%s\t\t%s\t\t%s\t\t%s' % (card['name'], card['phone_num'], card['qq_num'], card['e_mail']))
            process_card(card)
        else:
            print(f'未找到{find_name}的名片信息！')
    print('-' * 20)


def amend(info, tip_message):
    ret = input(tip_message)
    if len(ret) > 0:
        return ret
    else:
        return info


def process_card(card):
    print(card)
    opt = input('请选择要执行的操作：\n0、退出\t\t1、修改\t\t2、删除\n')
    if opt == '1':
        print('修改')
        card['name'] = amend(card['name'], '请输入姓名：')
        card['phone_num'] = amend(card['phone_num'], '请输入电话号码：')
        card['qq_num'] = amend(card['qq_num'], '请输入QQ号码：')
        card['e_mail'] = amend(card['e_mail'], '请输入电子邮箱：')
    elif opt == '2':
        print('删除')
        card_list.remove(card)
        print('删除成功！')


card_list = []
while True:
    BCMS_menu()
    option = input('请输入选项：')
    print(f'您的选择是：{option}')
    if option in ['1', '2', '3']:
        # TODO 主要内容
        if option == '1':
            create_card()
        elif option == '2':
            show_info()
        elif option == '3':
            search_card()
    elif option == '0':
        print('欢迎下次使用【名片管理系统】')
        break
    else:
        print('输入选项错误！请重新输入：')
