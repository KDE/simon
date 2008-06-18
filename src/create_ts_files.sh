find . -iname "*.pri" -exec cat {} \; | grep --invert-match "include" > unified_all.pro
cat src.pro >> unified_all.pro
lupdate unified_all.pro
