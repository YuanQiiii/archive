# 定义一个计算函数,以实现对给定数据的加减计算.形式变量amount代表金额,id_kind代表支出或收入,值为0或1,id_class代表账户类别,值为1,2,3,4
# 形式函数id_application代表用途,值为1,2,3,4,5,6,采用字典
# 邮政储蓄卡对应1,微信对应2,支付宝对应3,现金对应4,采用字典
amo = None
id_cls = None
id_ap = None
ap0_dict = {'1': '餐饮', '2': '交通', '3': '服务', '4': '购物', '5': '娱乐', '6': '交际'}
ap1_dict = {'1': '来自家庭', '2': '来自学校', '3': '来自自己'}
id_class_dict = {'1': '邮政储蓄卡', '2': '微信', '3': '支付宝', '4': '现金'}


def account_get():  # 获取四个参数的值,支持一定的异常报告能力
    global amo, id_cls, id_ap
    id_kd = input('请输入收支类型(收入为1,支出为0)')
    if id_kd not in ['0', '1']:
        print('无效的id_kind')
    else:
        variable_tips_id_cls = '请输入您的账户类型{0}'.format(str(id_class_dict))
        id_cls = input(variable_tips_id_cls)
        if id_cls not in ['1', '2', '3', '4']:
            print('无效的id_class')
        else:
            amo = input('请输入金额(数字)')
            try:
                amo = float(amo)
                if id_kd == '0':
                    variable_tips_id_application = '请输入支出用途{0}'.format(str(ap0_dict))  # 实现了动态提示词
                    id_ap = input(variable_tips_id_application)
                elif id_kd == '1':
                    variable_tips_id_application = '请输入收入来源{0}'.format(str(ap1_dict))
                    id_ap = input(variable_tips_id_application)
                else:
                    print('无效的id_application')
            except ValueError:
                print('无效的amount')
    return [amo, id_kd, id_cls, id_ap]


def account_calculator(amount, id_kind, id_class):  # 根据给定的参数和读取的数据计算各个账户类型的余额,支持百分比和简单图表显示
    with open('account_class.txt', 'a+') as fp1:
        content = fp1.read()
        if not bool(content):
            c_amount1 = c_amount2 = c_amount3 = c_amount4 = 0
        else:
            c_amount1, c_amount2, c_amount3, c_amount4 = content.split()
            c_amount1 = float(c_amount1)
            c_amount2 = float(c_amount2)
            c_amount3 = float(c_amount3)
            c_amount4 = float(c_amount4)
    if id_class == '1':
        if id_kind == '0':
            c_amount1 -= amount
        else:
            c_amount1 += amount
    elif id_class == '2':
        if id_kind == '0':
            c_amount2 -= amount
        else:
            c_amount2 += amount
    elif id_class == '3':
        if id_kind == '0':
            c_amount3 -= amount
        else:
            c_amount3 += amount
    elif id_class == '4':
        if id_kind == '0':
            c_amount4 -= amount
        else:
            c_amount4 += amount
    with open('account_class.txt', 'w') as fp2:
        print(c_amount1, ' ', c_amount2, ' ', c_amount3, ' ', c_amount4, file=fp2)
    t_c_amount = c_amount1 + c_amount2 + c_amount3 + c_amount4
    print('您的邮政储蓄卡余额为', c_amount1, ',占总余额的', percentage(c_amount1, t_c_amount))
    graph(c_amount1, t_c_amount)
    print('您的微信余额为', c_amount2, ',占总余额的', percentage(c_amount2, t_c_amount))
    graph(c_amount2, t_c_amount)
    print('您的支付宝余额为', c_amount3, ',占总余额的', percentage(c_amount3, t_c_amount))
    graph(c_amount3, t_c_amount)
    print('您的现金余额为', c_amount4, ',占总余额的', percentage(c_amount4, t_c_amount))
    graph(c_amount4, t_c_amount)
    print('您的所有账户余额为', t_c_amount)


def account_details(amount, id_kind, id_class, id_application):    # 通过给定的参数生成数据写入账本
    from datetime import date
    today = str(date.today())
    today = today.split('-')
    today = ''.join(today)
    if id_kind == '0':
        id_application = ap0_dict[id_application]
    elif id_kind == '1':
        id_application = ap1_dict[id_application]
    if id_kind == '0':
        id_kind = '支出'
    elif id_kind == '1':
        id_kind = '收入'
    else:
        print('无效id_kind')
    id_class = id_class_dict[id_class]
    with open('account_book.txt', 'a+') as fp2:
        print(today, ' ', id_kind, ' ', id_class, ' ', amount, ' ', id_application, file=fp2)
        print(today, ' ', id_kind, ' ', id_class, ' ', amount, ' ', id_application)


def account_statistics(amount, id_kind, id_application):    # 通过给定的参数和读取的数据对每一个收入的来源和支出的用途统计,支持简单的图表
    if id_kind == '0':
        with open('account_out_application.txt', 'a+') as fp2:
            content1 = fp2.read()
            if not bool(content1):
                ap1_amount1 = ap1_amount2 = ap1_amount3 = ap0_amount4 = ap0_amount5 = ap0_amount6 = 0
            else:
                ap1_amount1, ap1_amount2, ap1_amount3, ap0_amount4, ap0_amount5, ap0_amount6 = content1.split()
                ap1_amount1 = float(ap1_amount1)
                ap1_amount2 = float(ap1_amount2)
                ap1_amount3 = float(ap1_amount3)
                ap0_amount4 = float(ap0_amount4)
                ap0_amount5 = float(ap0_amount5)
                ap0_amount6 = float(ap0_amount6)
        if id_application == '1':
            ap1_amount1 += amount
        elif id_application == '2':
            ap1_amount2 += amount
        elif id_application == '3':
            ap1_amount3 += amount
        elif id_application == '4':
            ap0_amount4 += amount
        elif id_application == '5':
            ap0_amount5 += amount
        elif id_application == '6':
            ap0_amount6 += amount
        with open('account_out_application.txt', 'w') as fp2:
            print(ap1_amount1, ' ', ap1_amount2, ' ', ap1_amount3, ' ', ap0_amount4, ' ', ap0_amount5, ' ', ap0_amount6,
                  file=fp2)
        t_ap1_in_amount = ap1_amount1 + ap1_amount2 + ap1_amount3 + ap0_amount4 + ap0_amount5 + ap0_amount6
        print('您的餐饮支出为', ap1_amount1, ',占总支出的', percentage(ap1_amount1, t_ap1_in_amount))
        graph(ap1_amount1, t_ap1_in_amount)
        print('您的交通支出为', ap1_amount2, ',占总支出的', percentage(ap1_amount2, t_ap1_in_amount))
        graph(ap1_amount2, t_ap1_in_amount)
        print('您的服务支出为', ap1_amount3, ',占总支出的', percentage(ap1_amount3, t_ap1_in_amount))
        graph(ap1_amount3, t_ap1_in_amount)
        print('您的购物支出为', ap0_amount4, ',占总支出的', percentage(ap0_amount4, t_ap1_in_amount))
        graph(ap0_amount4, t_ap1_in_amount)
        print('您的娱乐支出为', ap0_amount5, ',占总支出的', percentage(ap0_amount5, t_ap1_in_amount))
        graph(ap0_amount5, t_ap1_in_amount)
        print('您的交际支出为', ap0_amount6, ',占总支出的', percentage(ap0_amount6, t_ap1_in_amount))
        graph(ap0_amount6, t_ap1_in_amount)
        print('您的总支出为', t_ap1_in_amount)
    elif id_kind == '1':
        with open('account_in_application.txt', 'a+') as fp2:
            content1 = fp2.read()
            if not bool(content1):
                ap1_amount1 = ap1_amount2 = ap1_amount3 = 0
            else:
                ap1_amount1, ap1_amount2, ap1_amount3 = content1.split()
                ap1_amount1 = float(ap1_amount1)
                ap1_amount2 = float(ap1_amount2)
                ap1_amount3 = float(ap1_amount3)
        if id_application == '1':
            ap1_amount1 += amount
        elif id_application == '2':
            ap1_amount2 += amount
        elif id_application == '3':
            ap1_amount3 += amount

        with open('account_in_application.txt', 'w') as fp2:
            print(ap1_amount1, ' ', ap1_amount2, ' ', ap1_amount3, file=fp2)
        t_ap1_in_amount = ap1_amount1 + ap1_amount2 + ap1_amount3
        print('您来自于家庭的收入为', ap1_amount1, ',占总收入的', percentage(ap1_amount1, t_ap1_in_amount))
        graph(ap1_amount1, t_ap1_in_amount)
        print('您来自于学校的收入为', ap1_amount2, ',占总收入的', percentage(ap1_amount2, t_ap1_in_amount))
        graph(ap1_amount2, t_ap1_in_amount)
        print('您来自于自己的收入为', ap1_amount3, ',占总收入的', percentage(ap1_amount3, t_ap1_in_amount))
        graph(ap1_amount3, t_ap1_in_amount)
        print('您的总收入为', t_ap1_in_amount)


def percentage(s_number, t_number):   # 用于计算一个值相对于另一个值的占比
    p = (s_number / t_number) * 100
    p = round(p, 2)
    p = str(p)
    p = p + '%'
    return p


def graph(s_number, t_number):
    p = (s_number / t_number) * 100
    np = int(p // 1)
    if p >= 0:
        if p > 0 and np == 0:
            np = 1
    print('-' * 100)
    print('*' * 100)
    print('*' * np)
    print('-' * 100)
