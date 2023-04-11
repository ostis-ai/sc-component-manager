### Initialize
To initialize your repo do:
 * make fork from `https://github.com/ostis-ai/sc-component_manager`
 * clone your fork to your machine and prepare
```sh
git clone git@github.com:yourlogin/sc-component-manager.git
cd sc-component_manager
git remote add upstream git@github.com:ostis-ai/sc-component_manager.git
```
### Update 
To update your main from `upstream` use:
```sh
git fetch upstream
git checkout main
git merge upstream/main
git push origin main
```

### Rebase
To rebase your branch to main:
```sh
git checkout yourbranch
git rebase main
```

If you have any problems, then:
* redo
```sh
git rebase --abort
```

### Common rules
* use `git rebase` instead of `merge`. [More documentation about this command](https://git-scm.com/docs/git-rebase) This command just try to apply your commits (from current branch to commits in specified branch)
