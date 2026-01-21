from clang.cindex import (
    BaseEnumeration,
    Cursor,
    conf,
    cursor_null_guard,
)


class UnaryOperator(BaseEnumeration):
    """
    Describes the UnaryOperator of a declaration
    """

    def __nonzero__(self):
        """Allows checks of the kind ```if cursor.unary_operator:```"""
        return self.value != 0

    Invalid = 0
    PostInc = 1
    PostDec = 2
    PreInc = 3
    PreDec = 4
    AddrOf = 5
    Deref = 6
    Plus = 7
    Minus = 8
    Not = 9
    LNot = 10
    Real = 11
    Image = 12
    Extension = 13
    Coawait = 14


@property
@cursor_null_guard
def unary_operator(self) -> UnaryOperator:
    """
    Retrieves the opcode if this cursor points to a unary operator
    :return:
    """
    if not hasattr(self, "_unaryopcode"):
        self._unaryopcode = conf.lib.clang_getCursorUnaryOperatorKind(self)
    return UnaryOperator.from_id(self._unaryopcode)


Cursor.unary_operator = unary_operator  # type: ignore

__all__ = ["UnaryOperator"]
