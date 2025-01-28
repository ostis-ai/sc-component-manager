This page describes rules to contribute changes and features by Pull Requests creating.

## Initialize

To initialize your repo do:

* Fork `https://github.com/ostis-ai/sc-component-manager`.
* Clone your fork to your machine and prepare (see [Readme](https://github.com/ostis-ai/sc-component-manager)).

```sh
git clone git@github.com:yourlogin/sc-component-manager.git
cd sc-component-manager
git remote add upstream git@github.com:ostis-ai/sc-component-manager.git
```

* To update your `main` from `upstream` use:

```sh
git fetch upstream
git checkout upstream/main
```

* Use `git rebase` instead of `merge`. See [documentation about this command](https://git-scm.com/docs/git-rebase). To rebase your branch against main use:

```sh
git checkout <yourbranch>
git rebase upstream/main
```

* If you have any problems, then redo:

```sh
git rebase --abort
```

* Or ask in [Element](https://app.element.io/index.html#/room/#ostis_tech_support:matrix.org).

## Commits message format

Each commit message should be formed as: `[tag1]...[tagN] Message text (#issue)`.

Message text should start from an upper case letter. If commit doesn't fix or implement any #issue, then it shouldn't 
be pointed in commit message.

Examples:
<pre>
[kb] Update test components specification
[init][test] Add unit test for init comand
[utils] Add get specification address util
</pre>

Possible tags:

  * `[init]` - changes in `init` comand;
  * `[install]` - changes in `install` module;
  * `[search]` - changes in `search` module;
  * `[utils]` - changes in `utils`;
  * `[keynodes]` - changes in `keynodes` module;
  * `[kb]` - changes in `kb` module;
  * `[downloader]` - changes in `downloader` module;
  * `[interface]` - changes in `command line interface` module;
  * `[review]` - commits with review fixes;
  * `[refactor]` - commits with some code refactoring;
  * `[changelog]` - use when you update changelog;
  * `[docs]` - use when you update documentation;
  * `[scripts]` - updates in the `sc-component_manager/scripts` files
  * `[ci]` - changes in `ci` configuration or scripts;
  * `[git]` - changes in `git` configuration;
  * `[cmake]` - changes in `cmake` build system.
  * `[tests]` - changes in `tests`.

Each commit in Pull Request should be an atomic. In other words, it should implement or fix one feature. For example:
<pre>
Last commit
...
[utils][test] Add unit test for get specification address util
[utils] Add get specification address util
...
Init commit
</pre>

In this example we add class to work with console (where implemented colored output), then in another commit we add implementation of colored log output.

***
Commits should be small in size excluding cases, with:

  * CodeStyle changes; 
  * Renames; 
  * Code formatting.

**Do atomic commits for each changes.** For example if you rename some members in `ClassX` and `ClassY`, then do two commits:

<pre>
[refactor] Rename members in ClassX according to codestyle
[refactor] Rename members in ClassY according to codestyle
</pre>

**Do not mix codestyle changes and any logical fixes in one commit.**

All commits that not applies to this rules should be split by this rules. Otherwise they will be rejected with Pull Request.

***
## Pull Request

Each Pull Request with many changes, that not possible to review (excluding codestyle, rename changes), will be rejected.

_**All commit, that not applies to these rules, should be split by these rules. Another way they will be rejected with Pull Request.**_

### Pull Request Preparation

 - Read rules to create PR in documentation;
 - Update changelog;
 - Update documentation;
 - Cover new functionality by tests;
 - Your code should be written according to a codestyle like in sc-machine (see [Codestyle rules](https://ostis-ai.github.io/sc-machine/dev/codestyle/)).

### Pull Request Creation

 - Create PR on GitHub;
 - Check that CI checks were passed successfully.

### Pull Request Review

 - Reviewer should test code from PR if CI don't do it;
 - Reviewer submit review as set of conversations;
 - Author make review fixes at `Review fixes` commits;
 - Author re-request review;
 - Reviewer resolve conversations if they were fixed and approve PR.
