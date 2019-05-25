#include <iostream>

using namespace std;

class Class
{
  private:

  int *data_;
  size_t num_of_els_ { 10 };

  bool data_moved_ { false };

  public:

  /*! \brief Default constructor.
   *  
  */
  Class(int fill)
  {
    this->data_ = new int[this->num_of_els_];

    for (size_t i { 0 }; i < this->num_of_els_; ++i)
    {
      this->data_[i] = { fill };
    }
  }

  /*! \brief Move constructor.
   *
   *  Takes rvalues only.
   */
  Class(Class&& c)
  {
    cout << "Moving data in constructor\n";
    this->data_   = { c.data_ };

    cout << "Invoking methods of donor class\n";
    c.data_moved_ = { true };
    c.~Class();
  }

  /*! \brief Copy constructor.
   *  
   *  Takes rvalues and lvalues, so need move constructor to separate behavior.
   */
  Class(Class const& c)
  {
    cout << "Copying data in constructor\n";

    this->num_of_els_ = { c.num_of_els_ };
    this->data_ = new int[this->num_of_els_];
    
    for (size_t i {}; i < this->num_of_els_; ++i)
    {
      this->data_[i] = c.data_[i];
    }
  }

  /*! \brief Move assingment operator.
   *
   *  Takes rvalues only.
   */
  Class& operator=(Class&& c)
  {
    cout << "Moving data in operator=\n";
    this->data_ = { c.data_ };

    cout << "Invoking methods of donor class\n";
    c.data_moved_ = { true };
    c.~Class();

    return *this;
  }

  /*! \brief Move assingment operator.
   *
   *  Takes rvalues and lvalues, so need move constructor to separate behavior.
   */
  Class& operator=(Class const& c)
  {
    cout << "Copying data in operator=\n";

    this->num_of_els_ = { c.num_of_els_ };
    this->data_ = new int[this->num_of_els_];

    for (size_t i{}; i < this->num_of_els_; ++i)
    {
      this->data_[i] = c.data_[i];
    }

    return *this;
  }

  /*! \brief Destructor.
   *
   *  Virtual destructor.
   */
  virtual ~Class()
  {
    if (this->data_moved_)
    {
      cout << "Setting pointer to nullptr\n";

      this->data_ = nullptr;
    }
    else
    {
      cout << "Deleting pointer\n";

      delete this->data_;
      this->data_ = nullptr;
    }
  }

  /*! \brief Print class data.
   *  
   */
  void Print()
  {
    if (nullptr == this->data_)
    {
      cerr << "Pointer to data is nullptr. Data ownership was probably moved, "
              "or memory was never allocated, or pointer was never initialized\n";

      return;
    }

    for (size_t i {}; i < this->num_of_els_; ++i)
    {
      cout << this->data_[i] << ' ';
    }

    cout << endl;
  }
};

int main()
{
  cout << "Creating c1\n";
  Class c1(1);
  cout << endl;

  cout << "Creating c2\n";
  // move(c1) is a rvalue.
  // Calling move constructor, if any, copy constructor otherwise.
  Class c2(move(c1));  
  cout << endl;

  cout << "Creating c3\n";
  // Operator= is here, but it's still constricting, not assigning.
  // Calling move constructor, if any, copy constructor otherwise.
  // move(c2) is a rvalue, calling move operator.
  Class c3 = move(c2);
  cout << endl;

  cout << "Creating c4\n";
  // Initialize first.
  Class c4(2);
  cout << "Assigning c4\n";
  // Now assigning.
  // Now move operator= can be invoked, if any exist.
  c4 = move(c3);
  cout << endl;

  cout << "Creating c5\n";
  Class c5(c4);  // c4 is a lvalue, calling copy constructor.
  cout << endl;

  cout << "Creating c6\n";
  // Operator= is here, but it's still constricting, not assigning.
  // Now can re-use c4! c4 is a lvalue, calling copy constructor.
  Class c6 = c4;
  cout << endl;

  cout << "Creating c7\n";
  // Initialize first.
  Class c7(3);
  cout << "Assigning c7\n";
  // Now assigning.
  // Now move operator= can be invoked, if any exist.
  // c4 is a lvalue, calling copy operator=.
  c7 = c4;
  cout << endl;

  cout << "c1 is moved from\n";
  c1.Print();
  cout << endl;

  cout << "c2 is moved from\n";
  c2.Print();
  cout << endl;

  cout << "c3 is moved from\n";
  c3.Print();
  cout << endl;

  cout << "c4\n";
  c4.Print();
  cout << endl;

  cout << "c5\n";
  c5.Print();
  cout << endl;

  cout << "c6\n";
  c6.Print();
  cout << endl;

  cout << "c7\n";
  c7.Print();
  cout << endl;
  
  cout << "c4 is still alive\n";
  c4.Print();
  cout << endl;

  cin.get();
}
