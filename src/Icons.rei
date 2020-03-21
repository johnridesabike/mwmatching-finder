module ChevronLeft: {
  [@bs.module "react-feather/dist/icons/chevron-left"] [@react.component]
  external make:
    (~className: string=?, ~style: ReactDOMRe.Style.t=?) => React.element =
    "default";
};

module ChevronDown: {
  [@bs.module "react-feather/dist/icons/chevron-down"] [@react.component]
  external make:
    (~className: string=?, ~style: ReactDOMRe.Style.t=?) => React.element =
    "default";
};

module Reason: {
  [@react.component]
  let make:
    (
      ~height: string=?,
      ~width: string=?,
      ~className: string=?,
      ~style: ReactDOMRe.Style.t=?,
      ~ariaLabel: string=?,
      ~ariaHidden: bool=?
    ) =>
    React.element;
};

module D3: {
  [@react.component]
  let make:
    (
      ~height: string=?,
      ~width: string=?,
      ~className: string=?,
      ~style: ReactDOMRe.Style.t=?,
      ~ariaLabel: string=?,
      ~ariaHidden: bool=?
    ) =>
    React.element;
};

module GitHub: {
  [@react.component]
  let make:
    (
      ~height: string=?,
      ~width: string=?,
      ~className: string=?,
      ~style: ReactDOMRe.Style.t=?,
      ~ariaLabel: string=?,
      ~ariaHidden: bool=?
    ) =>
    React.element;
};

module NodeJs: {
  [@react.component]
  let make:
    (
      ~height: string=?,
      ~width: string=?,
      ~className: string=?,
      ~style: ReactDOMRe.Style.t=?,
      ~ariaLabel: string=?,
      ~ariaHidden: bool=?
    ) =>
    React.element;
};

module SimpleIcons: {
  [@react.component]
  let make:
    (
      ~height: string=?,
      ~width: string=?,
      ~className: string=?,
      ~style: ReactDOMRe.Style.t=?,
      ~ariaLabel: string=?,
      ~ariaHidden: bool=?
    ) =>
    React.element;
};

module React: {
  [@react.component]
  let make:
    (
      ~height: string=?,
      ~width: string=?,
      ~className: string=?,
      ~style: ReactDOMRe.Style.t=?,
      ~ariaLabel: string=?,
      ~ariaHidden: bool=?
    ) =>
    React.element;
};
