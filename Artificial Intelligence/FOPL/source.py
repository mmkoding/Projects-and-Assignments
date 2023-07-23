substitution_mapping = {}


class Compound:
    def __init__(self, expr):
        self.expr = expr
        self.open_paren = self.expr.find('(')
        self.close_paren = self.expr.rfind(')')
        
    def op(self):
        operator_end = self.expr.index('(')
        return self.expr[1:operator_end] if self.expr[0] == '~' else self.expr[:operator_end]

    def args(self):
        if self.open_paren != -1 and self.close_paren != -1:
            arg_str = self.expr[self.open_paren + 1:self.close_paren]
            arg_list = arg_str.split(',')
            return arg_list if len(arg_list) > 1 else arg_list[0]
        else:
            return None



def is_variable(term):
    return term.islower() and (len(term) == 1 or term[1] != '(')

def is_compound_term(term):
    if '~' in term:
        term = term[1:]  # Remove the '~' at the beginning if it exists

    open_parenthesis = term.find('(')
    close_parenthesis = term.rfind(')')

    return (
        open_parenthesis != -1 and
        close_parenthesis != -1 and
        close_parenthesis > open_parenthesis and
        close_parenthesis - open_parenthesis > 1
    )

def unify_variable(variable, term, substitutions):
    if variable in substitution_mapping and (variable, substitution_mapping[variable]) in substitutions:
        return unify(substitution_mapping[variable], term, substitutions)
    if term in substitution_mapping and (term, substitution_mapping[term]) in substitutions:
        return unify(variable, substitution_mapping[term], substitutions)
    substitution_mapping[variable] = term
    substitutions.append((variable, term))
    return substitutions



def unify(expression1, expression2, substitutions):
    if substitutions is None:
        return None
    if expression1 == expression2:
        return substitutions
    if not isinstance(expression1, list) and is_variable(expression1):
        return unify_variable(expression1, expression2, substitutions)
    if not isinstance(expression2, list) and is_variable(expression2):
        return unify_variable(expression2, expression1, substitutions)
    if not isinstance(expression1, list) and not isinstance(expression2, list) and is_compound_term(expression1) and is_compound_term(expression2):
        compound1 = Compound(expression1)
        compound2 = Compound(expression2)
        return unify(compound1.args(), compound2.args(), unify(compound1.op(), compound2.op(), substitutions))
    if isinstance(expression1, list) and isinstance(expression2, list):
        return unify(expression1[1:], expression2[1:], unify(expression1[0], expression2[0], substitutions))
    return None



def remove_tautology(resolvents):
    to_be_removed = set()

    for i in resolvents:
        comp1 = Compound(i)
        for j in resolvents:
            comp2 = Compound(j)

            if comp1.op() == comp2.op() and i[0] != j[0]:
                theta = []
                substitution_mapping = {}
                unify_result = unify(i, j, theta)

                if unify_result:
                    to_be_removed.add(i)
                    to_be_removed.add(j)

    return resolvents.difference(to_be_removed)



def check_subsumption(resolvents, B, G):
    U = B.union(G)

    for clause in resolvents:
        for kb_clause in U:
            if len(kb_clause) == 1:
                c1 = Compound(clause)
                c2 = Compound(kb_clause[0])

                if clause == kb_clause or (c1.op() == c2.op() and clause[0] == kb_clause[0][0]):
                    args1 = c1.args()
                    args2 = c2.args()

                    if isinstance(args1, list) and isinstance(args2, list):
                        subsumed = all(is_variable(arg2) or arg1 == arg2 for arg1, arg2 in zip(args1, args2))
                        if subsumed:
                            return True
                    elif type(args1) == type(args2) and (is_variable(args2) or args1 == args2):
                        return True

    return False



def resolve_clauses(clause1, clause2):
    global variable_mapping
    resolved_clauses = set()

    for idx1, sub_clause1 in enumerate(clause1):
        for idx2, sub_clause2 in enumerate(clause2):
            compound1 = Compound(sub_clause1)
            compound2 = Compound(sub_clause2)

            if compound1.op() == compound2.op() and sub_clause1[0] != sub_clause2[0]:
                substitution_set = []
                variable_mapping = {}
                unify(sub_clause1, sub_clause2, substitution_set)
                clause1 = apply_unification(clause1, substitution_set)
                clause2 = apply_unification(clause2, substitution_set)
                clause1.pop(idx1)
                clause2.pop(idx2)

                combined_clauses = set(clause1).union(set(clause2))
                resolved_clauses.update(combined_clauses)

                if not resolved_clauses:
                    return 'empty'

                resolved_clauses = remove_tautology(resolved_clauses)
                return tuple(resolved_clauses)



def first_order_logic_resolution(knowledge_base, goal):
    new_resolvents = set()
    resolution_trace = []

    while True:
        for kb_clause in knowledge_base:
            for goal_clause in goal:
                resolvents = resolve_clauses(kb_clause, goal_clause)

                if resolvents == 'empty':
                    resolution_trace.append('+'.join(map(str, goal_clause)) + '$' + '+'.join(map(str, kb_clause)) + '$' + 'empty')
                    print(('yes', resolution_trace))
                    return True
                elif resolvents and resolvents in goal:
                    continue
                elif resolvents and not check_subsumption(resolvents, knowledge_base, goal):
                    resolution_trace.append('+'.join(map(str, goal_clause)) + '$' + '+'.join(map(str, kb_clause)) + '$' + '+'.join(
                        map(str, resolvents)))
                    new_resolvents.add(resolvents)

        if new_resolvents <= goal or new_resolvents <= knowledge_base:
            print(('no', []))
            return False

        goal.update(new_resolvents)





def apply_unification(clause_list, substitutions):
    updated_clause_list = []
    for clause in clause_list:
        updated_clause = clause
        for variable, replacement in substitutions:
            updated_clause = updated_clause.replace(variable, replacement)
        updated_clause_list.append(updated_clause)
    return updated_clause_list



def divide_into_clauses(logic_str):
    separator_positions = []
    parenthesis_count = 0
    for index, char in enumerate(logic_str):
        if char == '(':
            parenthesis_count += 1
        elif char == ')':
            parenthesis_count -= 1
        elif char == '+' and parenthesis_count == 0:
            separator_positions.append(index)

    if not separator_positions:
        return [logic_str]

    clauses = []
    end = len(logic_str) - 1
    separator_positions.append(end)
    for idx in range(len(separator_positions)):
        if idx == 0:
            clauses.append(logic_str[0:separator_positions[idx]])
        else:
            clauses.append(logic_str[separator_positions[idx - 1] + 1:separator_positions[idx] + 1])

    return clauses



def theorem_prover(B_list, G_list):
    B = [tuple(divide_into_clauses(lstr)) for lstr in B_list]
    G = [tuple(divide_into_clauses(lstr)) for lstr in G_list]

    first_order_logic_resolution(set(B), set(G))
    
    
#theorem_prover(["h(A,f(t))","i(z)+~h(z,f(B))","~i(y)+j(y)"],["~j(A)"])
#theorem_prover(["h(A,f(t))","i(z)+~h(z,f(B))","~i(y)+j(y)"],["j(A)"])
#theorem_prover(["f(T)", "~f(x)+b(x)"],["~b(T)"])
#theorem_prover(["f(A,B)","a(A)","~f(x,y)+p(x,y)","~p(x,y)+~a(x)+o(x,y)"],["~o(A,B)"])
#theorem_prover(["k(J,x)","k(y,K)"],["~k(J,K)"])
#theorem_prover(["k(y,K)","k(J,x)"],["~k(J,K)"])
#theorem_prover(["p(A,f(t,B))", "q(z,C)+~p(z,f(D,u))", "~q(x,y)+r(x,y)"],["~r(A,C)"])
#theorem_prover(["~p(x,y,z)+i(x)+g(z,y)","p(T,F,E)", "~i(T)"],["~g(E,F)"])