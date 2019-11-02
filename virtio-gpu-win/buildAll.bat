@echo off
for %%D in (viogpu) do (
  pushd %%D
  call buildAll.bat
  if errorlevel 1 goto :eof
  popd
)
