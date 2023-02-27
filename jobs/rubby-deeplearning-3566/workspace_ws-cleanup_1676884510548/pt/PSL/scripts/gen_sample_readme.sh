ROOT_DIR=$(cd `dirname $0`; pwd)
cd $ROOT_DIR/..
FILE=README.md
file_name=$1/$FILE

mkdir -p `dirname $file_name`
cp $FILE $file_name
sed -i '/### comple with shell/,$d' $file_name
sed -i '/comple/a\\ remark: move `sample/CMakeLists.txt.example` to `sample/CMakeLists.txt`' $file_name
sed -i '/base on dl/d' $file_name
