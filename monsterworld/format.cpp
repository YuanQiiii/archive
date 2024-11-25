#include <algorithm> // 用于排序
#include <iomanip>   // 用于操作输出格式
#include <iostream>  // 用于输入输出
#include <string>    //  用于使用字符串
#include <unordered_map>
#include <vector> // 用于使用动态数组

// 定义调试级别
#define DEBUG_LEVEL 0
// 根据调试级别决定是否打印调试信息
#if DEBUG_LEVEL > 0
#define DEBUG_PRINT(x) std::cout << x << std::endl
#else
#define DEBUG_PRINT(x)
#endif

using namespace std;
int M = 0, N = 0, K = 0, T = 0; // 生命元,城市数目,忠诚度下降量,总时间
// 双方制造的顺序
string redOrder[5] = {"iceman", "lion", "wolf", "ninja", "dragon"};
string blueOrder[5] = {"lion", "dragon", "ninja", "iceman", "wolf"};
// 全局时间
int all_time = 0;
int &getTime() { return all_time; };
int hour() { return all_time / 60; };
int minute() { return all_time % 60; };
bool timeLimit() { return 60 * hour() + minute() > T; } // 是否时间超出限制
void timeRadio() { // 按照格式输出时间,但是没有尾随空格
  cout << setw(3) << setfill('0') << hour();
  cout << ':';
  cout << setw(2) << setfill('0') << minute();
}
// 初始化全局变量
int dragonElements, ninjaElements, icemanElements, lionElements, wolfElements;
int dragonForce, ninjaForce, icemanForce, lionForce, wolfForce;
// 武器名称
string weaponName[3] = {"sword", "bomb", "arrow"};
// 武器类的声明
class
    Weapon; // 这样定义可以克服定义先后依赖的问题(后面只能使用指针来解决需要完整定义初始化的问题)
// 武士类的定义
class Warrior {
public:
  string name;               // 名称
  bool team;                 // red ==> true blue ==> false
  int order;                 // 序号
  int elements;              // 生命值
  int force;                 // 攻击力
  int position;              // 位置
  int weaponNumber[3] = {0}; // 各个武器数目
  vector<Weapon *> weapons; // 武器数组,建议使用erase方法操作动态数组
  void stepForward(
      vector<Warrior *> &
          stepWarriors) { // 用于武士前进,传入数组stepWarriors用于储存完成了前进操作的武士
    if (this->team) {
      position++;
      stepWarriors.push_back(this);
    } else {
      position--;
      stepWarriors.push_back(this);
    }
  }
  virtual void yell() {}                              // 用于dragon
  virtual bool rob(Warrior *target) { return false; } // 用于wolf
  virtual bool boomSafe() { return false; }           // 用于ninja
  virtual void stepElementsDown() {}                  // 用于iceman
  virtual void stepLoyaltyDown() {}                   // 用于lion
  virtual bool runAway() { return false; }            // 用于lion
  virtual void bornRadio() { // 用于广播武士生成,在lion派生类中有重载
    timeRadio();
    cout << (team ? " red " : " blue ") << this->name << ' ' << this->order
         << " born" << endl;
  }
  Warrior(int order, bool team, int elements, int force) {
    this->position = (team ? 0 : N + 1);
    this->order = order;
    this->elements = elements;
    this->force = force;
    this->team = team;
  }
  virtual ~Warrior() {}

  static bool cmpRadio(Warrior *a, Warrior *b) { // 自西向东,同意时间下先红后蓝
    if (a->position < b->position) {
      return true;
    } else if (a->position == b->position) {
      if (a->team) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
};
// 武器类的定义
class Weapon {
public:
  int order;    // 编号
  int force;    // 攻击力,随使用者刷新
  int elements; // 耐久
  static bool cmpRob(
      Weapon *a,
      Weapon *
          b) { // wolf的rob行为和杀死对方的缴获行为均需要调用(但是二者抢夺逻辑不一样)
    if (a->order < b->order) {
      return true;
    } else if (a->order == b->order) {
      return a->elements > b->elements;
    } else {
      return false;
    }
  }
  static bool cmpUse(Weapon *a,
                     Weapon *b) { // 战斗之前仅有一次的武器排序(作战准备)
    if (a->order < b->order) {
      return true;
    } else if (a->order == b->order) {
      return a->elements < b->elements;
    } else {
      return false;
    }
  }
  Weapon(int order, int userForce) {
    this->order = order;
    switch (order) {
    case 0:
      this->force = userForce * 2 / 10; // 防止数的表示出错
      this->elements = 10000;
      break;
    case 1:
      this->force = userForce * 4 / 10;
      this->elements = 1;
      break;
    case 2:
      this->force = userForce * 3 / 10;
      this->elements = 2;
      break;
    default: // 只有在所有条件都不满足的时候才执行
      break;
    }
  }
  ~Weapon() {}
  void resetWeapon(Warrior *user) {
    switch (this->order) {
    case 0:
      this->force = user->force * 2 / 10;
      break;
    case 1:
      this->force = user->force * 4 / 10;
      break;
    case 2:
      this->force = user->force * 3 / 10;
      break;
    }
  }
  void attack(Warrior *user, Warrior *target) {
    switch (this->order) {
    case 0:
      target->elements -= this->force;
      this->elements--; // 耐久减少
      break;
    case 1:
      target->elements -= this->force;
      if (!user->boomSafe()) { // 考虑ninja的bomb免伤
        user->elements -= this->force * 5 / 10;
      }
      this->elements--;
      break;
    case 2:
      target->elements -= this->force;
      this->elements--;
      break;
    default:
      break;
    }
  }
};
// 武器的检查和排序,传入一个武士对象的指针和一个比较函数的指针
void weaponCheckSort(Warrior *target, bool (*cmp)(Weapon *, Weapon *),
                     bool workModel = true) {
  for (auto it = target->weapons.begin(); it != target->weapons.end();
       /* 注意这里没有it++ */) {
    if ((*it) == nullptr) { // 针对武器的抢夺
      // erase返回指向被删除元素之后元素的迭代器
      it = target->weapons.erase(it);
    } else if ((*it)->elements == 0) { // 针对武器的耐久耗尽
      //            delete (*it);
      it = target->weapons.erase(it);
    } else { // 只有在不删除元素的情况下才自增迭代器
      ++it;
    }
  }
  if (workModel) {
    sort(target->weapons.begin(), target->weapons.end(), cmp);
  }
}
// 武士的派生类
// 以下的构造函数只需要写出两个参数 (序号, 队伍)
class Dragon : public Warrior {
public:
  Dragon(int order, bool team, int elements = dragonElements,
         int force = dragonForce)
      : Warrior(order, team, elements, force) {
    this->name = "dragon";
    this->weapons.push_back(new Weapon(this->order % 3, this->force));
    weaponNumber[this->order % 3] += 1;
  }
  void yell() override {
    if (this->elements > 0) {
      timeRadio();
      cout << (team ? " red dragon " : " blue dragon ") << this->order
           << " yelled in city " << this->position << endl;
    }
  }

  ~Dragon() {} // 这里不要尾随;或者漏写{},否则报错找不到虚函数表
};
class Iceman : public Warrior {
public:
  Iceman(int order, bool team, int elements = icemanElements,
         int force = icemanForce)
      : Warrior(order, team, elements, force) {
    this->name = "iceman";
    this->weapons.push_back(new Weapon(this->order % 3, this->force));
    weaponNumber[this->order % 3] += 1;
  }
  void stepElementsDown() override {
    this->elements -= this->elements * 1 / 10;
  }
  ~Iceman() {}
};
class Ninjia : public Warrior {
public:
  Ninjia(int order, bool team, int elements = ninjaElements,
         int force = ninjaForce)
      : Warrior(order, team, elements, force) {
    this->name = "ninja";
    this->weapons.push_back(new Weapon(this->order % 3, this->force));
    weaponNumber[this->order % 3] += 1;
    this->weapons.push_back(new Weapon((this->order + 1) % 3, this->force));
    weaponNumber[(this->order + 1) % 3] += 1;
  }
  bool boomSafe() override { return true; }
  ~Ninjia() {}
};
class Lion : public Warrior {
private:
  int loyalty = 0;

public:
  Lion(int order, bool team, int baseElements, int elements = lionElements,
       int force = lionForce) // 被创建之后需要特殊的广播 ==> 忠诚值
      : Warrior(order, team, elements, force) {
    this->loyalty = baseElements;
    this->name = "lion";
    this->weapons.push_back(new Weapon(this->order % 3, this->force));
    weaponNumber[this->order % 3] += 1;
  }

  bool runAway() override { // 可能在己方司令部逃跑 ==> 注意遍历范围
    if (loyalty <= 0 && position != (team ? N + 1 : 0)) {
      timeRadio();
      this->elements = 0; // 重定义逃跑为生命值变成0,统一武士的管理
      cout << (team ? " red lion " : " blue lion ") << this->order
           << " ran away" << endl;
      return true;
    }
    return false;
  }
  void stepLoyaltyDown() override { this->loyalty -= K; }
  void bornRadio() override {
    timeRadio();
    cout << (team ? " red " : " blue ") << this->name << ' ' << this->order
         << " born" << endl;
    cout << "Its loyalty is " << this->loyalty << endl;
  }
  ~Lion() {}
};
class Wolf : public Warrior {

public:
  Wolf(int order, bool team, int elements = wolfElements, int force = wolfForce)
      : Warrior(order, team, elements, force) {
    this->name = "wolf";
  }

  bool rob(Warrior *target) override {
    int robNumber = 0; // 抢夺的数量
    int targetOrder =
        2; // 要抢夺的目标武器序号(仅抢序号最小的武器,并且优先抢夺没有使用过的)
    if (target->name != "wolf" &&
        !target->weapons
             .empty()) { // 满足抢夺发生的条件:自己是wolf对方不是wolf并且对方有武器
      // 在wolf抢夺武器之前对已经对双方武器检查和排序(必要)
      weaponCheckSort(target, Weapon::cmpRob);
      for (auto &iter : target->weapons) {
        if (this->weapons.size() == 10) { // 武器达到10件,停止抢夺
          break;
        }
        if (iter->order <= targetOrder) {
          targetOrder = iter->order;
          this->weaponNumber[targetOrder]++;
          Weapon *temp = iter;
          temp->resetWeapon(this);
          iter = nullptr;
          this->weapons.push_back(temp);
          robNumber++;
        }
      }
      weaponCheckSort(target, Weapon::cmpUse); // 为被抢夺方去除被抢夺的武器
      timeRadio();
      cout << (team ? " red wolf " : " blue wolf ") << this->order << " took "
           << robNumber << ' ' << weaponName[targetOrder] << " from "
           << (!team ? "red " : "blue ") << target->name << ' ' << target->order
           << " in city " << this->position << endl;
      return true;
    } else {
      return false;
    }
  }
  ~Wolf() {}
};

class Headquarter {
public:
  bool team;
  int elements = 0;
  int currentOrder = 1;
  bool makeRight = true;
  vector<Warrior *> warriors;
  Headquarter(bool team, int elements = M) {
    this->elements = elements;
    this->team = team;
  }
  void elementsRadio() {
    timeRadio();
    cout << ' ' << this->elements << " elements in " << (team ? "red" : "blue")
         << " headquarter" << endl;
  }
  bool create(const string &name) { // 节省内存开销
    if (makeRight) {                // 是否有生产权力
      if (name == "dragon" && this->elements >= dragonElements) {
        this->elements -= dragonElements; // 减去对应生命元
        warriors.push_back(new Dragon(currentOrder++, this->team));
        (*(warriors.end() - 1))->bornRadio(); // 武士生成广播
      } else if (name == "ninja" && this->elements >= ninjaElements) {
        this->elements -= ninjaElements;
        warriors.push_back(new Ninjia(currentOrder++, this->team));
        (*(warriors.end() - 1))->bornRadio();
      } else if (name == "iceman" && this->elements >= icemanElements) {
        this->elements -= icemanElements;
        warriors.push_back(new Iceman(currentOrder++, this->team));
        (*(warriors.end() - 1))->bornRadio();
      } else if (name == "lion" && this->elements >= lionElements) {
        this->elements -= lionElements;
        warriors.push_back(
            new Lion(currentOrder++, this->team, this->elements));
        (*(warriors.end() - 1))->bornRadio();
      } else if (name == "wolf" && this->elements >= wolfElements) {
        this->elements -= wolfElements;
        warriors.push_back(new Wolf(currentOrder++, this->team));
        (*(warriors.end() - 1))->bornRadio();
      } else {
        return false;
      }
      return true;
    } else {
      makeRight = false;
      return false;
    }
  }
};
// 用于武士的检查和排序操作
void warriorCheckSort(vector<Warrior *> &warriors,
                      bool (*cmp)(Warrior *, Warrior *),
                      bool workModel = true) {
  for (auto it = warriors.begin(); it != warriors.end();
       /* 注意这里没有it++ */) {
    if ((*it) == nullptr) { // 防止空指针
      // erase返回指向被删除元素之后元素的迭代器
      it = warriors.erase(it);
    } else if ((*it)->elements <=
               0) { // 针对武士生命值耗尽和武士逃跑(被重定义为生命值为0)
      // delete (*it);
      it = warriors.erase(it);
    } else { // 只有在不删除元素的情况下才自增迭代器
      ++it;
    }
  }
  if (workModel) {
    sort(warriors.begin(), warriors.end(), cmp);
  }
}
// 更新武器数目
void updateWeaponNumber(Warrior *warrior) {
  warrior->weaponNumber[0] = 0;
  warrior->weaponNumber[1] = 0;
  warrior->weaponNumber[2] = 0;
  for (auto iter : warrior->weapons) {
    warrior->weaponNumber[iter->order]++;
  }
}
// 寻找在目标城市的武士,存在则返回对应的指针,不存在则返回空指针
Warrior *findWarriorInCity(vector<Warrior *> &warriors, int city) {
  // 建立从城市编号到Warrior*的映射。
  unordered_map<int, Warrior *> cityToWarriorMap;

  // 填充映射，假设每个城市只有一个战士。
  for (auto &warrior : warriors) {
    if (warrior) { // 确保不是空指针。
      cityToWarriorMap[warrior->position] = warrior;
    }
  }

  // 使用城市编号在映射中直接查找Warrior。
  auto iter = cityToWarriorMap.find(city);
  if (iter != cityToWarriorMap.end()) {
    // 找到了对应城市的Warrior。
    return iter->second;
  } else {
    // 没有战士在该城市。
    return nullptr;
  }
}

// 武士状态广播
void statementRadio(const vector<Warrior *> &redWarriors,
                    const vector<Warrior *> &blueWarriors) {
  vector<Warrior *> tempRadio(redWarriors.begin(), redWarriors.end());
  tempRadio.insert(tempRadio.end(), blueWarriors.begin(), blueWarriors.end());
  warriorCheckSort(tempRadio, Warrior::cmpRadio);
  for (auto iter : tempRadio) {
    timeRadio();
    cout << (iter->team ? " red " : " blue ") << iter->name << ' '
         << iter->order << " has " << iter->weaponNumber[0] << " sword "
         << iter->weaponNumber[1] << " bomb " << iter->weaponNumber[2]
         << " arrow and " << iter->elements << " elements" << endl;
  }
}
// 战斗结果广播(掠夺行为在这里完成),并且在结束前检查和更新双方的武器,但是并未对武士进行删除操作
void fightRadio(Warrior *redWarrior, Warrior *blueWarrior) {
  timeRadio();
  if (redWarrior->elements <= 0 && blueWarrior->elements <= 0) {
    // 全死亡
    cout << " both red " << redWarrior->name << ' ' << redWarrior->order
         << " and blue " << blueWarrior->name << ' ' << blueWarrior->order
         << " died in city " << redWarrior->position << endl;
  } else if (redWarrior->elements <= 0) {
    // 红方死亡
    weaponCheckSort(redWarrior, Weapon::cmpRob);
    for (auto weapon : redWarrior->weapons) {
      if (blueWarrior->weapons.size() < 10) {
        Weapon *temp = weapon;
        weapon = nullptr;
        temp->resetWeapon(blueWarrior); // 掠夺武器并重置攻击力
        blueWarrior->weapons.push_back(temp);
      }
    }
    cout << (blueWarrior->team ? " red " : " blue ") << blueWarrior->name << ' '
         << blueWarrior->order << " killed "
         << (redWarrior->team ? "red " : "blue ") << redWarrior->name << ' '
         << redWarrior->order << " in city " << redWarrior->position
         << " remaining " << blueWarrior->elements << " elements" << endl;
    blueWarrior->yell();
  } else if (blueWarrior->elements <= 0) {
    // 蓝方死亡
    weaponCheckSort(blueWarrior, Weapon::cmpRob);
    for (auto weapon : blueWarrior->weapons) {
      if (redWarrior->weapons.size() < 10) {
        Weapon *temp = weapon;
        weapon = nullptr;
        temp->resetWeapon(redWarrior); // 掠夺武器并重置攻击力
        redWarrior->weapons.push_back(temp);
      }
    }
    cout << (redWarrior->team ? " red " : " blue ") << redWarrior->name << ' '
         << redWarrior->order << " killed "
         << (blueWarrior->team ? "red " : "blue ") << blueWarrior->name << ' '
         << blueWarrior->order << " in city " << redWarrior->position
         << " remaining " << redWarrior->elements << " elements" << endl;
    redWarrior->yell();
  } else {
    // 全生存(武器不一定被耗尽)
    cout << " both " << (redWarrior->team ? "red " : "blue ")
         << redWarrior->name << ' ' << redWarrior->order << " and "
         << (blueWarrior->team ? "red " : "blue ") << blueWarrior->name << ' '
         << blueWarrior->order << " were alive in city " << redWarrior->position
         << endl;
    redWarrior->yell();
    blueWarrior->yell();
  }
  // 播报完成之后,更新武士的武器状态和信息
  // 可以释放死亡对象的武器内存空间和耐久消失的武器内存空间
  weaponCheckSort(redWarrior, Weapon::cmpUse, false);
  weaponCheckSort(blueWarrior, Weapon::cmpUse, false);
  updateWeaponNumber(redWarrior);
  updateWeaponNumber(blueWarrior);
}
// 战争结束广播
void endRadio(const Warrior *warrior) {
  timeRadio();
  cout << (warrior->team ? " red " : " blue ") << warrior->name << ' '
       << warrior->order << " reached " << (warrior->team ? "blue" : "red")
       << " headquarter with " << warrior->elements << " elements and force "
       << warrior->force << endl;
  timeRadio();
  cout << (warrior->team ? " blue " : " red ") << "headquarter was taken"
       << endl;
}

// 武士前进信息广播 ==>
// 需要传入参数:完成前进操作的武士数组,(同时判断游戏是否结束 ==> true)
bool stepRadio(vector<Warrior *> &stepWarriors) {
  warriorCheckSort(stepWarriors, Warrior::cmpRadio);
  // 检查是否有占领行为发生,如果有则战争结束(假设有同时占领对方司令部的事件出现,则认为红方胜利
  // ==> 和排序函数相关)
  bool overFlag = false;
  for (auto iter : stepWarriors) {
    if (iter->position == ((iter->team) ? N + 1 : 0)) {
      endRadio(iter);
      overFlag = true;
      continue;
    }
    timeRadio();
    cout << (iter->team ? " red " : " blue ") << iter->name << ' '
         << iter->order << " marched to city " << iter->position << " with "
         << iter->elements << " elements and force " << iter->force << endl;
  }
  return overFlag;
}

// 武器获取函数,获取可用的武器,并在获取结束之后将index++,下次调用不用再对index做自增操作了
Weapon *weaponGet(Warrior *user, int &index) {
  Weapon *availableWeapon = nullptr;
  int length = user->weapons.size();
  // 9:40 应该为 1

  if (length == 0) { // 没有武器时函数终止,防止除0
    return availableWeapon;
  }
  int maxCircle = 0;
  DEBUG_PRINT("weaponGet");
  DEBUG_PRINT(length);
  while (!availableWeapon &&
         maxCircle < 10 * length) { // 一直找不到能使用的武器,死循环
    DEBUG_PRINT("weaponGetCircle");
    index %= length;
    if (!user->weapons
             [index]) { // 虽然做了一系列防止访问空指针的操作,但是以防万一
      index++;
      continue;
    }
    if (user->weapons[index]->elements > 0) {
      availableWeapon = user->weapons[index];
    }
    index++;
    maxCircle++;
  }
  return availableWeapon;
}
// 考虑双方武器攻击力为0且为sword,或者双方武器用尽,则需要避免死循环
inline bool specialCaseCheck(Warrior *redWarrior, Warrior *blueWarrior) {

  if (redWarrior->force == 0 && blueWarrior->force == 0) {
    for (auto iter : redWarrior->weapons) {
      if ((iter->order != 0) && iter->elements > 0) {
        return false;
      }
    }
    for (auto iter : blueWarrior->weapons) {
      if ((iter->order != 0) && iter->elements > 0) {
        return false;
      }
    }
    return true;
  } else if (redWarrior->force != 0 && blueWarrior->force != 0) {
    int availableRedTimes = 0, availableBlueTimes = 0; // 记录可用次数
    for (auto iter : redWarrior->weapons) {
      availableRedTimes += iter->elements;
    }
    for (auto iter : blueWarrior->weapons) {
      availableBlueTimes += iter->elements;
    }
    if (availableRedTimes || availableBlueTimes) { // 仍有可用次数
      return false;
    } else {
      return true;
    }
  } else if (redWarrior->force != 0) { //  red != 0     blue == 0
    int availableRedTimes = 0, availableBlueTimes = 0; // 记录可用次数
    for (auto iter : redWarrior->weapons) {
      availableRedTimes += iter->elements;
    }
    for (auto iter : blueWarrior->weapons) {
      if ((iter->order != 0) && iter->elements > 0) {
        return false;
      }
    }
    if (availableRedTimes) { // 仍有可用次数
      return false;
    } else {
      return true;
    }
  } else {                                             // red == 0     blue != 0
    int availableRedTimes = 0, availableBlueTimes = 0; // 记录可用次数
    for (auto iter : redWarrior->weapons) {
      if ((iter->order != 0) && iter->elements > 0) {
        return false;
      }
    }
    for (auto iter : blueWarrior->weapons) {
      availableBlueTimes += iter->elements;
    }
    if (availableBlueTimes) { // 仍有可用次数
      return false;
    } else {
      return true;
    }
  }
  return false;
}

void battle(vector<Warrior *> &redWarriors, vector<Warrior *> &blueWarriors,
            vector<Warrior *> &redRemain, vector<Warrior *> &blueRemain) {
  // 10-min
  DEBUG_PRINT("inbattle");
  DEBUG_PRINT(getTime());
  // 全局时间变化,timeRadio的依赖,如果事件无法发生,为后续事件做准备
  getTime() += 25;
  if (timeLimit()) {
    return;
  }
  for (int city = 1; city <= N; city++) {
    Warrior *redWarrior = findWarriorInCity(redWarriors, city);
    Warrior *blueWarrior = findWarriorInCity(blueWarriors, city);
    if (redWarrior == nullptr ||
        blueWarrior == nullptr) { // 一方为空则停止本轮,进入下一轮
      continue;
    }
    if (redWarrior->weapons.empty() && blueWarrior->weapons.empty()) {
      redRemain.push_back(redWarrior);
      blueRemain.push_back(blueWarrior);
      continue;
    }
    // 35-min
    if (redWarrior->elements > 0 && blueWarrior->elements > 0) {
      redWarrior->rob(
          blueWarrior); // 函数内完成了抢夺之前的排序和抢夺之后为了作战的武器排序
      blueWarrior->rob(redWarrior);
    }

    // 全局时间变动
    getTime() += 5;
    if (timeLimit()) {
      getTime() -= 5;
      continue;
    }
    // 判断攻击先手
    bool redFirst = true;
    if (city % 2 == 1) {
      redFirst = true;
    } else {
      redFirst = false;
    }
    // 初始化武器索引和武器指针
    int ri = 0, bi = 0;

    // 考虑特殊情况
    int maxTurn = 0; // 针对有攻击力,但是取整后为0的情况
    while (redWarrior->elements > 0 && blueWarrior->elements > 0 &&
           maxTurn < 1000) {
      weaponCheckSort(redWarrior, Weapon::cmpUse);
      weaponCheckSort(blueWarrior, Weapon::cmpUse);
      Weapon *redWeapon = nullptr;
      Weapon *blueWeapon = nullptr;
      maxTurn++;
      if (redFirst) {
        redWeapon = weaponGet(redWarrior, ri);
        if (redWeapon) {
          redWeapon->attack(redWarrior, blueWarrior);
        }
        // 9:40 此时的dragon7 有 10
        // 9:40 此时的ninja4 有 10
        redFirst = false;
        DEBUG_PRINT("(RED)");
        DEBUG_PRINT((redWeapon == nullptr));
      } else {
        blueWeapon = weaponGet(blueWarrior, bi);
        if (blueWeapon) {
          blueWeapon->attack(blueWarrior, redWarrior);
        }
        redFirst = true;
        DEBUG_PRINT("(BLUE)");
        DEBUG_PRINT((blueWeapon == nullptr));
      }
      if (specialCaseCheck(redWarrior, blueWarrior)) { // 死循环的可能
        break;
      }
    }
    // 延迟报告战斗结果
    redRemain.push_back(redWarrior);
    blueRemain.push_back(blueWarrior);
    // 该城市结束,循环到下一个城市,重置全局时间
    getTime() -= 5;
  }
  getTime() += 5;
  DEBUG_PRINT("endbattle");
  DEBUG_PRINT(getTime());
}

int main() {

  int t = 0; // 记录样例数目
  cin >> t;
  for (int i = 1; i <= t; i++) {
    cin >> M >> N >> K >> T;
    cin >> dragonElements >> ninjaElements >> icemanElements >> lionElements >>
        wolfElements;
    cin >> dragonForce >> ninjaForce >> icemanForce >> lionForce >> wolfForce;
    cout << "Case " << i << ':' << endl;
    // 初始化两军基地
    Headquarter red(true, M), blue(false, M);
    int redIndex = 0, blueIndex = 0;
    // 初始化全局时间
    getTime() = 0;
    while (true) {
      // 00-min,双方生成武士
      if (!red.create(redOrder[redIndex])) {
        red.makeRight = false;
      }
      if (!blue.create(blueOrder[blueIndex])) {
        blue.makeRight = false;
      }
      redIndex++;
      blueIndex++;
      redIndex %= 5;
      blueIndex %= 5;
      // 时间变动
      getTime() += 5;
      if (timeLimit()) {
        break;
      }
      // 05-min
      DEBUG_PRINT(getTime());
      for (auto iter : red.warriors) {
        iter->runAway();
      }
      for (auto &iter : blue.warriors) {
        iter->runAway();
      }
      warriorCheckSort(red.warriors, Warrior::cmpRadio, false);
      warriorCheckSort(blue.warriors, Warrior::cmpRadio, false);
      // 时间变动
      getTime() += 5;
      DEBUG_PRINT(getTime());
      if (timeLimit()) {
        break;
      }
      // 10-min
      vector<Warrior *> stepWarriors;
      for (auto &iter : red.warriors) {
        iter->stepElementsDown();
        iter->stepLoyaltyDown();
        iter->stepForward(stepWarriors);
      }
      for (auto &iter : blue.warriors) {
        iter->stepElementsDown();
        iter->stepLoyaltyDown();
        iter->stepForward(stepWarriors);
      }
      if (stepRadio(stepWarriors)) {
        break;
      }

      // 10-40min
      if (timeLimit()) {
        break;
      }
      vector<Warrior *> redRemain, blueRemain;
      battle(red.warriors, blue.warriors, redRemain, blueRemain);
      for (int l = 0; l < redRemain.size(); l++) {
        fightRadio(redRemain[l], blueRemain[l]);
      }
      DEBUG_PRINT(getTime());
      // 时间变动
      getTime() += 10;
      if (timeLimit()) {
        break;
      }
      DEBUG_PRINT(getTime());
      // 50-min
      red.elementsRadio();
      blue.elementsRadio();
      // 时间变动
      getTime() += 5;
      DEBUG_PRINT(getTime());
      if (timeLimit()) {
        break;
      }
      // 55-min
      statementRadio(red.warriors, blue.warriors);
      // 时间变动
      getTime() += 5;
      DEBUG_PRINT(getTime());
      if (timeLimit()) {
        break;
      }
      // 60-min ==> 0-min
    }
  }
}