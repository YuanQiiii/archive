# 程序主体 V1.0.0

import modules
if __name__ == '__main__':
    while True:
        amo, id_kd, id_cls, id_ap = modules.account_get()
        modules.account_calculator(amo, id_kd, id_cls)
        modules.account_details(amo, id_kd, id_cls, id_ap)
        modules.account_statistics(amo, id_kd, id_ap)
        choice = input('是否退出y/n')
        if choice == 'y':
            break
        elif choice == 'n':
            continue
