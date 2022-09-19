# [sc-component-manager](https://docs.google.com/document/d/18IdwjANJfFpixAw82PFefkrjVBEgQKnLfWUAqH6LYnc/edit?usp=sharing)

# Installation
## Using ostis-web-platform
- Open scripts/prepare.sh and change sc-machine branch to sc-component-manager (clone_project https://github.com/ostis-ai/sc-machine.git sc-machine sc-component-manager)

# Usage
## Commands
- components init - downloading specifications from repos.txt sources to _specifications_ folder (automatically created if is not exists)
- components search [--author \<author\>][--class \<class\>][--explanation \<"explanation"\>]

## Repos structure
## Repository file
File repos.txt Contains of two sections: [components] and [repositories].
[repositories] are links for source (github, google drive etc.) with repos.txt file, [components] are links to source with specification.txt file of component.

Example of repository (repos.txt file)

```txt
[repositories]
https://github.com/MksmOrlov/components-repo-example

[components]
https://github.com/MksmOrlov/cat-kb-component
https://drive.google.com/file/d/1eUxPBd3VKNFSDEgolz11EQdEmBJMFYIk/view?usp=sharing
```

## Specification file
Example of components specification (specification.scs file)

``` scs
lion_specification
<- concept_reusable_component;
<- concept_atomic_component;
<- concept_kb_component;

=> nrel_author: ... (* -> Orlov;; -> Petrov;;*);
=> nrel_explanation: [Semantic neighbourhood of lion.]
                     (*<- lang_en;;*);

=> nrel_inner_deps: ...;
=> nrel_outer_deps: ...;
=> nrel_upstream_url: [https://github.com/MksmOrlov/lion-kb-component]
                      (*<- github_url;;*);
=> nrel_installation_command: 
[git clone https://github.com/MksmOrlov/lion-kb-component ; ./build_kb.sh];;
```
