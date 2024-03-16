// https://stackoverflow.com/questions/17378961/elegant-way-to-implement-extensible-factories-in-c

#include <iostream>
#include <utility>
#include <memory>

// Type list stuff: (perhaps use an existing library here)
class EmptyType {};

template<class T1, class T2 = EmptyType>
struct TypeList
{
  using Head = T1;
  using Tail = T2;
};

template<class... Etc>
struct MakeTypeList;

template <class Head>
struct MakeTypeList<Head>
{
  using Type = TypeList<Head>;
};

template <class Head, class... Etc>
struct MakeTypeList<Head, Etc...>
{
  using Type = TypeList<Head, typename MakeTypeList<Etc...>::Type >;
};

// Calling produce
template<class TList, class BaseType>
struct Producer;

template<class BaseType>
struct Producer<EmptyType, BaseType>
{
   template<class... Args> static std::shared_ptr<BaseType> Produce(Args... args)
   {
      return nullptr;
   }
};

template<class Head, class Tail, class BaseType>
  struct Producer<TypeList<Head, Tail>, BaseType>
{
   template<class... Args> static std::shared_ptr<BaseType> Produce(Args... args)
   {
      std::shared_ptr<BaseType> b = Head::Produce(args...);
      if(b != nullptr)
        return b;
      return Producer<Tail, BaseType>::Produce(args...);
   }
};

// Generic AbstractFactory:
template<class BaseType, class Types>
  struct AbstractFactory {
     using ProducerType = Producer<Types, BaseType>;
     
     template<class... Args> static std::shared_ptr<BaseType> Produce(Args... args)
     {
        return ProducerType::Produce(args...);
     }
  };
