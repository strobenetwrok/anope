/* OperServ core functions
 *
 * (C) 2003-2012 Anope Team
 * Contact us at team@anope.org
 *
 * Please read COPYING and README for further details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 *
 */
/*************************************************************************/

#include "module.h"

static int do_modload(User * u);
static void myOperServHelp(User * u);

/**
 * Create the command, and tell anope about it.
 * @param argc Argument count
 * @param argv Argument list
 * @return MOD_CONT to allow the module, MOD_STOP to stop it
 **/
int AnopeInit(int argc, char **argv)
{
    Command *c;

    moduleAddAuthor("Anope");
    moduleAddVersion
        (VERSION_STRING);
    moduleSetType(CORE);

    c = createCommand("MODLOAD", do_modload, is_services_root, -1, -1, -1,
                      -1, OPER_HELP_MODLOAD);
    moduleAddCommand(OPERSERV, c, MOD_UNIQUE);

    moduleSetOperHelp(myOperServHelp);

    return MOD_CONT;
}

/**
 * Unload the module
 **/
void AnopeFini(void)
{

}


/**
 * Add the help response to anopes /os help output.
 * @param u The user who is requesting help
 **/
static void myOperServHelp(User * u)
{
    if (is_services_root(u)) {
        notice_lang(s_OperServ, u, OPER_HELP_CMD_MODLOAD);
    }
}

/**
 * The /os modload command.
 * @param u The user who issued the command
 * @param MOD_CONT to continue processing other modules, MOD_STOP to stop processing.
 **/
static int do_modload(User * u)
{
    char *name;

    name = strtok(NULL, "");
    if (!name) {
        syntax_error(s_OperServ, u, "MODLOAD", OPER_MODULE_LOAD_SYNTAX);
        return MOD_CONT;
    }
    if (!queueModuleLoad(name, u))
        notice_lang(s_OperServ, u, OPER_MODULE_LOAD_FAIL, name);

    return MOD_CONT;
}
