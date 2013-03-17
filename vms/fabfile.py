from fabric.api import task, sudo
from fabtools.vagrant import vagrant
from fabtools import require


@task
def install_dependencies():
    sudo('apt-get update')
    sudo('apt-get upgrade --yes')
    # require.deb.packages([
    #     'make',
    #     'build-essential',
    #     'ocaml',
    #     'camlp4',
    #     'ocaml-findlib',
    #     'zlib1g-dev',
    # ])
