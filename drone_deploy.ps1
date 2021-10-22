cmdkey /generic:git:https://github.com /user:$Env:GH_USERNAME /pass:$Env:GH_TOKEN
git config --local user.name "drone-ci"
git config --local user.email "ci@enlyze.com"

git clone https://github.com/enlyze/ucrt-patched
cd ucrt-patched
copy ..\libucrt-patched.lib
git add libucrt-patched.lib
git commit -m "$Env:DRONE_COMMIT_MESSAGE"
git push
