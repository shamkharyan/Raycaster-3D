#ifndef CONTROLER_H_
#define CONTROLER_H_

class Controler
{
public:
    static Controler& get()
    {
        static Controler instance;
        return instance;
    }
    void run();
private:
    Controler();
    Controler(const Controler&) = delete;
    Controler& operator=(const Controler&) = delete;
};

#endif // CONTROLER_H_
