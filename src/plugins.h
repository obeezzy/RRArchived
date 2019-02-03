#ifndef PLUGINS_H
#define PLUGINS_H


class Plugins
{
public:
    Plugins();

    static void registerTypes();
    static void registerFonts();
    static void initLogging();
};

#endif // PLUGINS_H
