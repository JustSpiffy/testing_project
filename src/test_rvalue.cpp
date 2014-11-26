#include <iostream>
#include <string>
#include <vector>

class SomeObject
{
public:
    SomeObject() : m_name("AnObject") { std::cout << "[SomeObject] Constructed" << std::endl;}

    SomeObject(const SomeObject & rhs) : m_name(rhs.m_name) { std::cout << "[SomeObject] Copied " << m_name << std::endl; }

    void setName(const std::string & name) { m_name = name; }
private:
    std::string m_name;

};

class SomeAggregator
{
public:
    SomeAggregator() : m_listObjects() { };

    void pushObject(const SomeObject & object) { m_listObjects.push_back(object); }

    void emplaceObject(const SomeObject && object) { m_listObjects.emplace_back(object); }

private:
    std::vector<SomeObject> m_listObjects;
};

int main(int argc, char **argv) {
    std::cout << "Hello world" << std::endl;

    SomeAggregator aggr;
    SomeObject a, b;
    a.setName("a");
    b.setName("b");

    aggr.pushObject(a);
    aggr.emplaceObject(SomeObject());

    return 1;
}
