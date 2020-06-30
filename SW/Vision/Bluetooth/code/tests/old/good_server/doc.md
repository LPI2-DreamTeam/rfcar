
# Table of Contents

1.  [Folder struct](#org8f21174)
2.  [Git (intro)](#orgc839762)
    1.  [Git Basics](#org6315893)
        1.  [Git basics](#orgfd6f18e)
        2.  [Git Tutorial](#org0550786)
        3.  [Contribute to someone's repository](#org9edba71)
    2.  [Git workflow](#orgafddd6a)
3.  [Project](#org564024e)
    1.  [Functionalities <code>[0/6]</code>](#org05a8eb9)



<a id="org8f21174"></a>

# Folder struct

-   `rd.md`: proj's readme file
-   `C-Examples-Book`: contains examples from the Book
-   `doc`: proj's documentation
-   `code`: code's documentation
    -   `src`: contains the source code files for the project
        -   `Makefile`: file containing the rules to compile the proj
        -   `test-Makefile`: folder to test the makefile with 1 class `test`


<a id="orgc839762"></a>

# Git (intro)


<a id="org6315893"></a>

## Git Basics


<a id="orgfd6f18e"></a>

### Git basics

[src](https://git-scm.com/book/en/v2/Getting-Started-Git-Basics)
Git has 3 main states that your files can reside in:

-   *committed*: data is safely stored in your local database
-   *modified*: file has changed but it has not yet been commited to the local
    database
-   *staged*: a modified file was **marked** in its current version to go into the
    next commit snapshot

![img](doc/img/git-sections.png)

This leads us to the main sections of a Git proj:

1.  **Git directory**: the Git directory is where Git stores the metadata and
    object database for your project. This is the most important part of Git, and
    it is what is copied when you clone a repository from another computer.

2.  **Working tree**: the working tree is a *single checkout of one version of the
    project*. These files are pulled out of the compressed database in the Git
    directory and placed on disk for you to use or modify.

3.  **Staging area**: the staging area is a file, generally contained in your Git
    directory, that stores information about what will go into your next
    commit. Its technical name in Git parlance is the “index”, but the phrase
    “staging area” works just as well.

The basic Git workflow goes something like this:

1.  You modify files in your working tree.

2.  You selectively stage just those changes you want to be part of your next commit, which adds only those changes to the staging area.

3.  You do a commit, which takes the files as they are in the staging area and stores that snapshot permanently to your Git directory.


<a id="org0550786"></a>

### [Git Tutorial](https://kbroman.org/github_tutorial/)


<a id="org9edba71"></a>

### [Contribute to someone's repository](http://kbroman.org/github_tutorial/pages/fork.html)


<a id="orgafddd6a"></a>

## Git workflow

O branch estável será o `master` gerido pelo Zé. Este conterá apenas o código
estável. Assim, para desenvolver qq funcionalidade, cada elemento do grupo
deverá fazer o branch do `master`, atribuindo-lhe um nome relativo à
funcionalidade implementada, p.ex. `search-user`. Este branch será depois
`pushed` para o repositório `master`, sendo que será aceite se estiver de acordo
com as especificações, ou serão sugeridas alterações que o elemento deverá
implementar.

1.  Branch (`git checkout <branchname>`)
2.  Develop code
3.  Add to tracking (`git add file`)
4.  Add to *staging area* (transitory) (`git commit -m "Added search User
       function"`)
5.  If the code is good, upload to remote repository. (`git push <branchname`).


<a id="org564024e"></a>

# Project

**Tema**: *Chat Bluetooth de curto alcance*


<a id="org05a8eb9"></a>

## Functionalities <code>[0/6]</code>

-   [ ] Verificar estado da comunicação BT
-   [ ] Iniciar/Parar comunicação BT
-   [ ] Pesquisar dispositivos na proximidade
-   [ ] Emparelhar com um dispositivo
-   [ ] Enviar msg, lida do utilizador, para o dispositivo emparelhado
-   [ ] Receber msg do dispositivo emparelhado e apresentar ao utilizador

