
#ifndef _JOBS_H_
#define _JOBS_H_

class BecherBuilding;
class Troll;

struct Job
{
	enum Type
	{
		jtNone,
		jtPrines,
		jtOdnes,
		jtWork,
	} type;
	class BecherBuilding * owner;
	union {
		struct {
			class Store * store;
			ESurType surtype;
			int num;
		};
	};
};


class MainSklad
{
	// seznam skladu a surovin
public:

};

#endif // _JOBS_H_

