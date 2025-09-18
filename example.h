#ifndef EXAMPLE_CLASS_H
#define EXAMPLE_CLASS_H


class Example {
    public:
        int num;
        int* getPointerOfNum();
        int getNum();

        Example getReferenceToSelf();
};

#endif 