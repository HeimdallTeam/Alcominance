
#ifndef _WOOD_H_
#define _WOOD_H_


class tStrom : public tStatic
{
public:
	tStrom(float sx,float sy);
	~tStrom();

	virtual void Delete() { delete this; }
};

#endif // _WOOD_H_

