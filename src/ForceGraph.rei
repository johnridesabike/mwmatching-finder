[@react.component]
let make:
  (
    ~graph: Graph.t,
    ~mates: Blossom.Match.t(string, Blossom.Match.String.Cmp.identity),
    ~width: int,
    ~height: int,
    ~children: React.element=?,
    ~style: ReactDOMRe.Style.t=?
  ) =>
  React.element;
