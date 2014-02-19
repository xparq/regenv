@echo off

echo Project name....................: %PROJECT_NAME%
echo Project tag.....................: %PROJECT_TAG%

call __get_project_version
echo Current (or last named) baseline: %__project_version%
