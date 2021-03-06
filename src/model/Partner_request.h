#pragma once

#include "specializations.h"

#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/Types>

namespace dbo = Wt::Dbo;

class Assignment;
class Session;
class Submission;
class User;

class Partner_request
{
public:
    Partner_request() {}

    static dbo::ptr<Partner_request>
    create(Session&,
           const dbo::ptr<User>& requestor,
           const dbo::ptr<User>& requestee,
           const dbo::ptr<Assignment>&);

    void confirm(Session&) const;

    const dbo::ptr<User>& requestor() const { return requestor_; }
    const dbo::ptr<User>& requestee() const { return requestee_; }
    const dbo::ptr<Assignment>& assignment() const { return assignment_; }

    static dbo::ptr<Partner_request>
    find_by_requestor(Session&,
                      const dbo::ptr<User>&, const
                      dbo::ptr<Assignment>&);

    static dbo::collection<dbo::ptr<Partner_request>>
    find_by_requestee(Session&,
                      const dbo::ptr<User>&, const
                      dbo::ptr<Assignment>&);

private:
    dbo::ptr<User> requestor_;
    dbo::ptr<User> requestee_;
    dbo::ptr<Assignment> assignment_;

public:
    template<typename Action>
    void persist(Action& a)
    {
        dbo::belongsTo(a, requestor_, "requestor", dbo::OnDeleteCascade);
        dbo::belongsTo(a, requestee_, "requestee", dbo::OnDeleteCascade);
        dbo::belongsTo(a, assignment_, "assignment", dbo::OnDeleteCascade);
    }
};

DBO_EXTERN_TEMPLATES(Partner_request);
