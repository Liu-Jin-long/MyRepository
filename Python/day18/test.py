from threading import Thread


def SaySorry():
    while True:
        pass


if __name__ == '__main__':
    t1 = Thread(target=SaySorry)
    t1.start()
    t2 = Thread(target=SaySorry)
    t2.start()
    t3 = Thread(target=SaySorry)
    t3.start()
    t4 = Thread(target=SaySorry)
    t4.start()
    t5 = Thread(target=SaySorry)
    t5.start()
