#!/bin/bash
set -e

## start code-generator "^\\s *#\\s *"
# generate-getopt b:branch u:git-url n:n-commits p:prefix
## end code-generator
## start generated code
TEMP=$(getopt -o b:u:n:p: --long "branch:,git-url:,n-commits:,prefix:"  -n $(basename -- $0) -- "$@")
branch=
git_url=
n_commits=
prefix=
eval set -- "$TEMP"
while true; do
    case "$1" in
        -b|--branch)
            branch=$2
            shift 2
            ;;
        -u|--git-url)
            git_url=$2
            shift 2
            ;;
        -n|--n-commits)
            n_commits=$2
            shift 2
            ;;
        -p|--prefix)
            prefix=$2
            shift 2
            ;;
        --)
            shift
            break
            ;;
        *)
            die "internal error"
            ;;
    esac
done

## end generated code

if test -z "$prefix"; then
    prefix=qualcomm/oem
fi


target_branch=smandroid/8953-r07000.2-1

#cd /home/qi/project/odin/vendor/qcom/proprietary
cd /home/qi/project/odin-8953/vendor/qcom/proprietary

git checkout $subtree_p
for d in mm-camera mm-camerasdk; do
    subtree_p=$(git subtree split -P $d HEAD)
    git push s:qualcomm/platform/vendor/proprietary/$d $subtree_p:refs/heads/$target_branch
done

git filter-branch -f --index-filter "git rm -r --cached --ignore-unmatch mm-camera mm-camerasdk >/dev/null" HEAD
git push s:qualcomm/platform/vendor/proprietary HEAD:refs/heads/$target_branch

