;2010
for /f "delims=" %%a in ('dir/b/a-d/s "*.sdf"') do del /s /q "%%a"
for /f "delims=" %%a in ('dir/ad/b/s debug*') do rd /s /q "%%a"
for /f "delims=" %%a in ('dir/ad/b/s release*') do rd /s /q "%%a"
for /f "delims=" %%a in ('dir/ad/b/s ipch*') do rd /s /q "%%a"

for /f "delims=" %%a in ('dir/ad/b/s lib*') do rd /s /q "%%a"
for /f "delims=" %%a in ('dir/ad/b/s build*') do rd /s /q "%%a"
for /f "delims=" %%a in ('dir/ad/b/s localStorage*') do rd /s /q "%%a"