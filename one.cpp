#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <map>

// -------------------- Chapter 1 info...
struct CarInfo1
{
    std::string name {"default name"};
    unsigned year {1800};
    unsigned seats {4};
    double power {44.0};
};

void displayCarInfo1(const CarInfo1 &c1)
{
    std::cout << "CarInfo1 .... " << "name: " << c1.name <<
        "  year: " << c1.year << "  seats: " << c1.seats << 
        "  power: " << c1.power << std::endl;
}

void carInfo1Test()
{
    CarInfo1 firstCar;
    firstCar.name = "a Car";
    firstCar.year = 2001;
    firstCar.seats = 2;
    firstCar.power = 116.2;

    displayCarInfo1(firstCar);

    // Zero one out test (value initialization)
    CarInfo1 zeroCar{};
    displayCarInfo1(zeroCar);

    // Everything in the struct is public, so we can get at it
    CarInfo1 fullInit{"full init car", 1901, 1, 10.8};
    displayCarInfo1(fullInit);

    // We can also only specify the firts 2 values, and the others are implicit
    CarInfo1 partialInit{"partial car", 1902};
    displayCarInfo1(partialInit);
}
// -------------------- Chapter 1 info end...

// -------------------- Chapter 2 info...
size_t calcCheckSumStr(const std::string& s)
{
    return std::accumulate(s.begin(), s.end(), 0uz);
}

class DataPacket
{
    // class defaults to private
    std::string data_ {"dflt packet"};
    size_t checkSum_ {0};
    size_t serverId_ {200};

public:    
    DataPacket() = default;
    explicit DataPacket(const std::string& s, size_t anId)
        : data_{s}, checkSum_{calcCheckSumStr(data_)}, serverId_{anId}
    {}
    // This is chapter 3, but it fit nicely here
    // Also, notice the noexcept. When callng emplace_back() without it,
    // a copy constructor gets called. Having the noexcept calls the move
    // constructor and SHOULD be faster.
    // It's even more noticeable as the container (a vector or instance) is
    // expanded when size elements are added.
    // std::sort() and others should also benefit.
    DataPacket(DataPacket&& other) noexcept
        : data_(std::move(other.data_)),
        checkSum_(other.checkSum_), serverId_(other.serverId_)
    {
        std::cout << "DataPacket move constructor: " << data_ << "  checksum: " << checkSum_ << "  serverid: " << serverId_ << std::endl;
    }
    DataPacket(const DataPacket& other)
        : data_{other.data_}, checkSum_{other.checkSum_}, serverId_{other.serverId_}
    {
        std::cout << "DataPacket copy constructor: " << data_ << std::endl;        
    }
    DataPacket& operator=(const DataPacket&other)
    {
        if(this != &other)
        {
            data_ = other.data_;
            checkSum_ = other.checkSum_;
            serverId_ = other.serverId_;
        }

        std::cout << "Assignment operator for " << data_ << std::endl;;
        return *this;
    }
    // Move assignment operator - ch 3
    DataPacket& operator=(DataPacket&& other) noexcept
    {
        data_ = std::move(other.data_);
        checkSum_ = other.checkSum_;
        serverId_ = other.serverId_;

        std::cout << "Move Assignemnt operator for " << data_ << std::endl;
        return *this;
    }

    const std::string& getData() const {return data_;}
    void setData(const std::string& data)
    {
        data_ = data;
        checkSum_ = calcCheckSumStr(data_);
    }
    size_t getCheckSum() const {return checkSum_;}
    size_t getServerId() const {return serverId_;}
    void setServerId(size_t anId) { serverId_ = anId;}
};

void printDataPacket(const DataPacket& dp)
{
    std::cout << "DataPacket - data: " << dp.getData() <<
        "  checksum: " << dp.getCheckSum() <<
        "  serverId: " << dp.getServerId() << std::endl;
}

void chapter2()
{
    DataPacket d1;
    d1.setData("data packet set");
    printDataPacket(d1);

    DataPacket d2{"direct init pkt", 100};
    printDataPacket(d2);

    DataPacket d3("constructor pkt", 400);
    printDataPacket(d3);

    std::cout << "move packet move construction" << std::endl;
    DataPacket mv1{"move packet", 150};
    DataPacket mvDest{ std::move(mv1)};
    printDataPacket(mvDest);
    printDataPacket(mv1);

    std::cout << "second move construction" << std::endl;
    DataPacket mv2{"move packet", 151};
    // move copy
    DataPacket mvDest2 = std::move(mv2);
    printDataPacket(mvDest2);
    printDataPacket(mv2);

    std::cout << "move assignment" << std::endl;
    DataPacket mv3{"move packet assign", 152};
    DataPacket mvDest3;
    mvDest3 = mv3;
    printDataPacket(mvDest2);
    printDataPacket(mv2);

    // Another Chapter 3 thing. When copy vs assignment
    // gets called. 
    DataPacket mvSrc{"move source", 175};
    // This will generate copy
    DataPacket cpD = d1;
    // This will generate assignment
    mvDest = mvSrc;
    printDataPacket(mvDest);
}
// -------------------- Chapter 2 info end...

// -------------------- Chapter 6 info
// This section and/or methods will (at least in the beginning)
// be more of a "notes" section
void chapter6()
{
    // I knew a lot of this already, but noting it for later
    // xvalue and prvalue were new to me, i just knew them as rvalue.

    // The AAA rule (triple A).Almost Always Auto.
    
    // Maps are a good example
    std::map<std::string, int> mapping{{"first", 1,}, {"second", 2}};

    // auto here is so much easier to type and get right than defining the iterator
    auto it1 = mapping.find("second");

    // This code would even be "wrong". Because "string" is not const, it makes
    // a copy of the elements each time through. Notice the address changes
    for(const std::pair<std::string, int>& elem: mapping)
    {
        std::cout << "elem string: " << elem.first << "  address: " << &elem.first << std::endl;
    }

    // auto gets it right, and the address changes
    for(const auto& elem: mapping)
    {
        std::cout << "elem string: " << elem.first << "  address: " << &elem.first << std::endl;
    }

    // Lvalue - an expression representing an object with a persistent memory address.
    // xvalue - an expression thta represents a temporary object, such as the return of a function
    // that returns by value or an object that's about to be moved.
    // prvalue - an expression whose evaluation initializes an object o a bit-field, or computes
    // the value of theoperandof an operator.

    std::string h{"Hello"}; // h is an lvalue
    //static_cast<int&&>(h); // xvalue
    int x = 5 + 6; // 5 + 6 is a prvalue
    int y = 42; // y is an lvalue, 42 is a prvalue

    // Binding to traverse a map....
    // The trick below can be applied to other data structures.
    const std::map<std::string, int> mapCityPopulation{ {"Beijing", 21'707'000}, {"London", 8'787'892}, {"New York", 8'622'698}};
    for(const auto&[city, population] : mapCityPopulation)
    {
        std::cout << "city: " << city << "  population: " << population << std::endl;
    }
}
// -------------------- Chapter 2 info end...

int main()
{
    carInfo1Test();
    std::cout << std::endl;
    chapter2();
    chapter6();

    return 0;
}

