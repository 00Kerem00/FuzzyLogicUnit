#pragma once

class RuleTerm
{
public:
	virtual void terminateTermContext() = 0;
	virtual ~RuleTerm() {}
	virtual float getValue() = 0;
};