# Usage

## Commands

| Command              | Abbreviation                             | Description                                                                                       | Flags                                                                                                                          |
|:---------------------|:-----------------------------------------|:--------------------------------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------|
| `components init`    | `ci`, <br/> `comp init`                  | Download specifications from repositories.                                                        |                                                                                                                                |
| `components search`  | `cs`, <br/> `comp search`, <br/> `comp s` | Search a component specification in the knowledge base by author, class, key sc-element, explanation, note, purpose, or main identifier substring. | `[--author <author>]` <br/> `[--class <class>]` <br/> `[--key <key>]` <br/> `[--explanation <explanation>]` <br/> `[--note <note>]` <br/> `[--purpose <purpose>]` <br/> `[--main-id <main identifier>]` |
| `components install` | `cinst`, <br/> `comp inst`               | Install components by its system identifier or install the set of components.                     | `[--idtf <system_idtf>]` or `[<system_idtf>]` - install by an identifier <br/> `[--set <set>]` - install the set of components |

!!! Note
    If you are searching a string, you can put this string in quotes.
!!! Note
    `knowledge-base/specifications.scs` contains the example of a repository specification.
  
## Debug Logs

To see sc-component-manager debug logs set `log_level` to `Debug` in `[sc-memory]` section of `sc-component-manager.ini`

## Repository and components

### Repository specification

The repository specification describes two primary sections: components and repositories:

- Components section represents a list of components that have addresses to Github repositories that contain component sources and their specifications.
- Repositories section includes addresses to Github repositories that contain other repository specifications.

See example:

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

The component specification is a description of reusable component. 

See example:

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
