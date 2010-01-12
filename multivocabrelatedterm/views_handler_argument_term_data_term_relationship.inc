<?php
/**
 */
class views_handler_argument_term_data_term_relationship extends views_handler_argument {
  function query() {
    $this->ensure_my_table();
    $args = array($this->argument,$this->argument);
    $this->query->add_where(0, "$this->table_alias.tid in (SELECT tid2 from {term_relation} where tid1 = %d) OR $this->table_alias.tid in (SELECT tid1 from {term_relation} where tid2 = %d)", $args);
  }

  function title() {
    $term = taxonomy_get_term($this->argument);
    if (!empty($term)) {
      return check_plain($term->name);
    }
    // TODO review text
    return t('No name');
  }
}
