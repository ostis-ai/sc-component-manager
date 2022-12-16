# sc-component-manager

# Installation
## Using ostis-web-platform
- Install with sc-machine branch **feature/component_manager**
- To include components-manager kb, add 'sc-machine/sc-tools/sc-component-manager/kb' to platform repo.path file

# Usage
## Commands
- components init - downloading specifications from repos.txt sources to _specifications_ folder (automatically created if is not exists)
- components search [--author \<author\>][--class \<class\>][--explanation \<"explanation"\>]
- components install [--idtf \<system_idtf\>]

## Repos structure
## Repository file
File repos.txt Contains of two sections: [components] and [repositories].
[repositories] are links for source (GitHub, google drive etc.) with repos.txt file, [components] are links to source with specification.txt file of component.

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
cat_specification
    <- concept_reusable_component_specification;;

cat_specification = [*
concept_cat
    <- concept_reusable_component;
    <- concept_atomic_reusable_component;
    <- concept_independent_reusable_component;
    <- concept_reusable_kb_component;
    <- concept_reusable_dynamically_installed_component;
    <- concept_reusable_source_code_component;

    => nrel_sc_identifier: [Cat specification](* <- lang_en;; *);
    => nrel_key_sc_element: concept_animal;
    => nrel_purpose: [Cat specification is needed to design knowledge bases about animal world, pets and zoo.](* <- lang_en;; *);
    => nrel_explanation: [Meow meow meow!] (*<- lang_en;; *);
    => nrel_authors: ... (* -> Orlov;; *);
    => nrel_component_dependencies: ... (* <- empty_set;; *);

    => nrel_component_address: [https://github.com/MksmOrlov/cat-kb-component] (* <- github_url;; *);
    => nrel_installation_method: ... (* <- concept_component_dynamically_installed_method;; *);;
*];;
```
