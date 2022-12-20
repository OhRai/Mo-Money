#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 1
//

Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ):

        symbol{ticker_symbol},
        day{day_date},
        month{month_date},
        year{year_date},
        trans_type{buy_sell_trans ? "Buy" : "Sell"},
        shares{number_shares},
        amount{trans_amount},

        acb{},
        acb_per_share{},
        share_balance{},
        cgl{},

        p_next {nullptr}, 

        trans_id{assigned_trans_id} { ++assigned_trans_id; }

// Destructor
// TASK 1
//

Transaction::~Transaction() {
  delete p_next;
  p_next = nullptr;
}

// Overloaded < operator.
// TASK 2
//

bool Transaction::operator<( Transaction const &other ) {
  // Years are equal
  if ( get_year() == other.get_year() ) {
    
    // Months are equal
    if ( get_month() == other.get_month() ) {

      // Days are equal
      if ( get_day() == other.get_day() ) {
        return ( get_trans_id() > other.get_trans_id() );
      }

      // Left day is less than right day
      else if ( get_day() < other.get_day() ) {
        return true;
      }
    }

    // Left month is less than right month
    else if ( get_month() < other.get_month() ) {
      return true;
    }    
  }

  // Left year is less than right year
  else if ( get_year() < other.get_year() ) {
    return true;
  }

  return false;
}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//

History::History():
  p_head{nullptr} {}


// Destructor
// TASK 3
//

History::~History() {
  delete p_head;
  p_head = nullptr;
}

// read_history(...): Read the transaction history from file.
// TASK 4
//

void History::read_history() {
  ece150::open_file();

  // While being able to go to the next entry in the text file
  while ( ece150::next_trans_entry() ) {

    // Insert a new transaction
    insert( new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()) );
  }

  ece150::close_file();
  
}

// insert(...): Insert transaction into linked list.
// TASK 5
//

void History::insert( Transaction *p_new_trans ) {

  // p_next is the next node
  // p_head is the head of the linked list
  // p_new_trans is the node that is being inserted in the list

  //  Set the new trans to nullptr
  p_new_trans->set_next(nullptr);

  // if the head of the linked list is null, assign new trans to the head
  if ( p_head == nullptr ) {
    p_head = p_new_trans;
  } 
  
  else {
    // Checks if the node is pointing to another node. If not, then set it to null (it reaches the end)
    Transaction *temp {p_head};

    while ( temp->get_next() != nullptr ) {
      
      temp = temp->get_next();
      
    }

    temp->set_next(p_new_trans);
  }

}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

void History::sort_by_date() {

  Transaction *p_old {get_p_head()};
  Transaction *p_new {nullptr};

  while ( p_old != nullptr ) {

    Transaction *p_old_next{p_old->get_next()};
    Transaction *p_new_next{nullptr};

    // if the new list is empty
    if ( p_new == nullptr) {
      p_new = p_old;
    }

    // if the old ptr is earlier than the new ptr
    else if ( *p_old < *p_new ) {
      p_new_next = p_new; 
      p_new = p_old;
    }

    // if the next ptr on the new list is pointing to nothing
    else if ( p_new->get_next() == nullptr ) {
      p_new->set_next(p_old);
    }

    // finding where to put the old ptr into the new list
    else {
      Transaction *temp{p_new};
      while ( temp->get_next() != nullptr ) {

        if ( *p_old < *temp->get_next() ) {
          p_new_next = temp->get_next();
          break;
        }
        
        temp = temp->get_next();
      }

      temp->set_next(p_old);
    }

    p_old->set_next(p_new_next);
    p_old = p_old_next;
  }

  p_head = p_new;
}


// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl() {
  Transaction *temp{p_head};
  Transaction *prev{nullptr};
  double acb{0};
  unsigned int shares{0};
  double acb_ratio{0};
  double cgl{0};

  while ( temp != nullptr ) {

    // Buy Case
    if ( temp->get_trans_type() == true ){
      acb = acb + temp->get_amount();
      shares = shares + temp->get_shares();
      acb_ratio = acb / shares;

      temp->set_acb(acb);
      temp->set_share_balance(shares);
      temp->set_acb_per_share(acb_ratio);     
    }

    // Sell Case
    else {
      acb = prev->get_acb() - (temp->get_shares() * prev->get_acb_per_share());
      shares = prev->get_share_balance() - temp->get_shares();
      acb_ratio = acb / shares;
      cgl = temp->get_amount() - (temp->get_shares() * prev->get_acb_per_share());

      temp->set_acb(acb);
      temp->set_share_balance(shares);
      temp->set_acb_per_share(acb_ratio);
      temp->set_cgl(cgl);
    }
    prev = temp;
    temp = temp->get_next();
  }

}


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl( unsigned int year ) {
  Transaction *temp{p_head}; 
  double cgl_total{0};

  while ( temp != nullptr) {

    if (temp->get_year() == year) {
      cgl_total = cgl_total + temp->get_cgl();
    }

    temp = temp->get_next();
  }

  return cgl_total;
}

// print() Print the transaction history.
//TASK 9
//

void History::print() {

  Transaction *temp {p_head};

  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  
  while ( temp != nullptr ) {
  
    temp->print();

    temp = temp->get_next();

  }
  
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;

}

Transaction *History::get_p_head() { return p_head; }
