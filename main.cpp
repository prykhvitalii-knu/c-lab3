#include <iostream>
#include <thread>
#include <vector>
#include <latch>
#include <syncstream>
#include <chrono>

using namespace std;



latch a_done(4);
latch b_done(8);
latch c_done(6);
latch d_done(6);
latch e_done(7);
latch f_done(7);
latch g_done(6);
latch h_done(7);
latch i_done(5);
latch j_done(9);
latch k_done(4);

void f(char name, int i) {
    osyncstream(cout) << "З набору " << name << " виконано дію " << i << ".\n";
}


void thread1_work() {
    for (int k = 1; k <= 8; ++k) {
        f('b', k);
        b_done.count_down();
    }
    c_done.wait();

    for (int k = 1; k <= 7; ++k) {
        f('f', k);
        f_done.count_down();
    }
    e_done.wait();

    for (int k = 1; k <= 7; ++k) {
        f('h', k);
        h_done.count_down();
    }
}

void thread2_work() {
    for (int k = 1; k <= 6; ++k) {
        f('c', k);
        c_done.count_down();
    }
    e_done.wait();
    f_done.wait();

    for (int k = 1; k <= 5; ++k) {
        f('i', k);
        i_done.count_down();
    }
    h_done.wait();

    for (int k = 1; k <= 4; ++k) {
        f('k', k);
        k_done.count_down();
    }
}

void thread3_work() {
    for (int k = 1; k <= 4; ++k) {
        f('a', k);
        a_done.count_down();
    }

    for (int k = 1; k <= 6; ++k) {
        f('d', k);
        d_done.count_down();
    }

    for (int k = 1; k <= 6; ++k) {
        f('g', k);
        g_done.count_down();
    }
}

void thread4_work() {
    a_done.wait();

    for (int k = 1; k <= 7; ++k) {
        f('e', k);
        e_done.count_down();
    }
    g_done.wait();

    for (int k = 1; k <= 9; ++k) {
        f('j', k);
        j_done.count_down();
    }
}

int main() {
    cout << "Обчислення розпочато.\n" << endl;

    thread t1(thread1_work);
    thread t2(thread2_work);
    thread t3(thread3_work);
    thread t4(thread4_work);

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();
    if (t4.joinable()) t4.join();

    cout << "Обчислення завершено.\n";

    return 0;
}