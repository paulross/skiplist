#!/bin/bash
#
# Builds orderedstructs for distribution
# Ref: https://packaging.python.org/tutorials/packaging-projects/
#
# Other references:
# https://kvz.io/bash-best-practices.html
# https://bertvv.github.io/cheat-sheets/Bash.html

set -o errexit  # abort on nonzero exitstatus
set -o nounset  # abort on unbound variable
set -o pipefail # don't hide errors within pipes

#PYTHON_VERSIONS=('3.6')
PYTHON_VERSIONS=('3.6' '3.7' '3.8' '3.9')
PYTHON_VENV_ROOT="${HOME}/pyenvs"
# Used for venvs
PROJECT_NAME="SkipList"

#printf "%-8s %8s %10s %10s %12s\n" "Ext" "Files" "Lines" "Words" "Bytes"

deactivate_virtual_environment() {
  if command -v deactivate &>/dev/null; then
    deactivate
  fi
}

create_virtual_environments() {
  deactivate_virtual_environment
  for version in ${PYTHON_VERSIONS[*]}; do
    echo "---> For Python version ${version}"
    venv_path="${PYTHON_VENV_ROOT}/${PROJECT_NAME}_${version}"
    if [ ! -d "${venv_path}" ]; then
      # Control will enter here if directory not exists.
      echo "---> Creating virtual environment at: ${venv_path}"
      "python${version}" -m venv "${venv_path}"
    fi
  done
}

remove_virtual_environments() {
  deactivate_virtual_environment
  for version in ${PYTHON_VERSIONS[*]}; do
    echo "---> For Python version ${version}"
    venv_path="${PYTHON_VENV_ROOT}/${PROJECT_NAME}_${version}"
    if [ -d "${venv_path}" ]; then
      # Control will enter here if directory exists.
      echo "---> Removing virtual environment at: ${venv_path}"
      rm --recursive --force -- "${venv_path}"
    fi
  done
}

create_bdist_wheel() {
  echo "---> Creating bdist_wheel for all versions..."
  for version in ${PYTHON_VERSIONS[*]}; do
    echo "---> For Python version ${version}"
    deactivate_virtual_environment
    venv_path="${PYTHON_VENV_ROOT}/${PROJECT_NAME}_${version}"
    if [ ! -d "${venv_path}" ]; then
      # Control will enter here if directory doesn't exist.
      echo "---> Creating virtual environment at: ${venv_path}"
      "python${version}" -m venv "${venv_path}"
    fi
    # https://stackoverflow.com/questions/42997258/virtualenv-activate-script-wont-run-in-bash-script-with-set-euo
    set +u
    source "${venv_path}/bin/activate"
    set +u
    echo "---> Installing everything via pip:"
    pip install -U pip setuptools wheel
    pip install -r requirements.txt
    echo "---> Result of pip install:"
    pip list
    echo "---> Running python setup.py develop:"
    python setup.py develop
    echo "---> Running tests:"
    pytest tests
    echo "---> Running setup for bdist_wheel:"
    python setup.py bdist_wheel
  done
}

create_sdist() {
  echo "---> Running setup for sdist:"
  python setup.py sdist
}

show_results_of_dist() {
  echo "---> dist/:"
  ls -l "dist"
  # Test from Test PyPi
  # pip install -i https://test.pypi.org/simple/orderedstructs
  echo "---> Ready for upload to test PyPi:"
  echo "---> pip install twine"
  echo "---> twine upload --repository testpypi dist/*"
  echo "---> Or PyPi:"
  echo "---> twine upload dist/*"
}

echo "===> Removing build/ and dist/"
#rm --recursive --force -- "build" "dist"
rm -rf -- "build" "dist"
create_virtual_environments
create_bdist_wheel
create_sdist
show_results_of_dist
#pip install twine
echo "===> All done"
