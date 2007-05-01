dnl Check for Hoe Libraries
dnl CHECK_HOE(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  HOE_CFLAGS
dnl  HOE_LIBS
AC_DEFUN([CHECK_HOE],
[dnl

AC_ARG_WITH(
    hoe,
    [AC_HELP_STRING([--with-hoe=PATH],[Path to the Hoe Engine prefix])],
    hoe_path="$withval",
    :)

dnl # Determine hoe lib directory
if test -z $hoe_path; then
    test_paths="$HOME/Hoe"
else
    test_paths="${hoe_path}"
fi

for x in $test_paths ; do
    AC_MSG_CHECKING([for hoe3d.h in ${x}/Hoe3D/include])
    if test -f ${x}/Hoe3D/include/hoe3d.h; then
        AC_MSG_RESULT([yes])
        save_CFLAGS=$CFLAGS
        save_LDFLAGS=$LDFLAGS
        CFLAGS="$CFLAGS"
        LDFLAGS="-L$x/lib $LDFLAGS"
dnl        AC_CHECK_LIB(lua5.1, luaL_newstate,
dnl            [
            HOE_LIBS="-L$x/bin -lHoeGame -lHoeCore"
            HOE_CFLAGS="-I$x/Hoe3D/include -I$x/HoeCore/include -I$x/HoeGame/include -I$x/HoeEditor/include"
dnl            ])
        CFLAGS=$save_CFLAGS
        LDFLAGS=$save_LDFLAGS
        break
    else
        AC_MSG_RESULT([no])
    fi
done

AC_SUBST(HOE_LIBS)
AC_SUBST(HOE_CFLAGS)

if test -z "${HOE_LIBS}"; then
  AC_MSG_NOTICE([*** HoeEngine not found.])
  ifelse([$2], , AC_MSG_ERROR([HoeEngine is required]), $2)
else
  AC_MSG_NOTICE([using '${HOE_CFLAGS}' for HoeEngine])
  ifelse([$1], , , $1) 
fi 
])
