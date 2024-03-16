# sc-component-manager

It is the repository for the Component manager of the [OSTIS Project](https://github.com/ostis-ai).
It allows ostis-systems to install reusable components for intelligent systems made with the [**OSTIS Technology**](https://github.com/ostis-ai).

## Installation

Using ostis-web-platform
```sh
git clone https://github.com/ostis-ai/ostis-web-platform
cd ostis-web-platform
./scripts/install_platform_with_component_manager.sh
```

## Build
To build sc-component-manager run build script:

```sh
./scripts/build_sc_component_manager.sh
```

To build knowledge base, run ```./build_kb.sh``` in ostis-web-platform scripts

## Post Install

### Debug Logs
To see sc-component-manager debug logs set `log_level` to `Debug` in `[sc-memory]` section of `sc-machine.ini`

### Common issues:
- Can't load a whole repo using the command `components install --idtf <component_identifier>`. There is only a repository directory without source files.
  
  **Solution**: please make sure your Git is configured to be able to use `git sparse-checkout`.
  ```sh
  git config --global core.sparseCheckoutCone true
  git config --global core.sparseCheckout true
  git config --global index.sparse true
  ```
  You can see more in the [official documentation](https://git-scm.com/docs/git-sparse-checkout).

## Usage

### Start sc-component manager

To start sc-component-manager run:

``./scripts/run_sc_component_manager.sh``

Possible run flags:
- `-c` <CONFIG_PATH>
- `-i` - interactive mode to run command manually;

### Commands
| Command              | Abbreviation                             | Description                                                                                       | Flags                                                                                                                      |
|:---------------------|:-----------------------------------------|:--------------------------------------------------------------------------------------------------|:---------------------------------------------------------------------------------------------------------------------------|
| `components init`    | `ci`, <br/> `comp init`                  | Download specifications from repositories.                                                        |                                                                                                                            |
| `components search`  | `cs`, <br/> `comp search`, <br/>`comp s` | Search a component specification in the knowledge base by author, class or explanation substring. | [--author \<author\>] <br/> [--class \<class\>] <br/> [--explanation \<"explanation"\>] or [--explanation \<explanation\>] |
| `components install` | `cinst`, <br/> `comp inst`               | Install components by its system identifier.                                                      | [--idtf \<system_idtf\>] or [\<system_idtf\>] - install by an identifier                                                   |
- Note: `kb/specifications.scs` contains the example of a repository specification.
  

## Repository and components

File specification.scs contains description of two sections: **components** and **repositories**.

**Repositories** have links for source on GitHub with specification file, **components** have links to source with specification of component.

### Repository specification

Example of repository (`specifications.scs`)

```scs
sc_component_manager_repository
  <- concept_repository;
  -> rrel_components_specifications: ..components_addresses;
  -> rrel_repositories_specifications: ..repositories_addresses;;

..components_addresses
  <- sc_node_tuple;
  -> knowledge_base_ims_specification
    (*
      <- concept_reusable_component_specification;;
      => nrel_alternative_addresses:
      ...
      (*
        <- sc_node_tuple;;
        -> rrel_1:
          ... 
          (*
            -> [https://github.com/ostis-ai/ims.ostis.kb]
              (*
                <- concept_github_url;;
              *);;
          *);;
      *);;
    *);
  -> cat_kb_component_spec
      (*
           <- concept_reusable_component_specification;;
           => nrel_alternative_addresses:
           ...
           (*
                <- sc_node_tuple;;
                -> rrel_1:
                ...
                (*
                  -> [https://github.com/MksmOrlov/cat-kb-component]
                      (*
                        <- concept_github_url;;
                      *);;
                *);;
            *);;
        *);;

..repositories_addresses
  -> ... 
      (*
      -> rrel_address:
          [https://github.com/MksmOrlov/components-repo-example];;
      *);;
```

### Component specification

Example of components specification (`specification.scs`)

```scs
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

    => nrel_component_address: [https://github.com/MksmOrlov/cat-kb-component] (* <- concept_github_url;; *);
    => nrel_installation_method: ... (* <- concept_component_dynamically_installed_method;; *);;
*];;
```
