if not "%__PROJECT_ROOT%"=="" goto end

call __get_parent_dir
call __get_parent_dir %__parent_dir%
set __PROJECT_ROOT=%__parent_fullpath_noslash%

:end
