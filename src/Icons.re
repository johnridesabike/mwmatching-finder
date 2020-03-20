module ChevronLeft = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (~className: string=?, ~style: ReactDOMRe.Style.t=?) => React.element =
    "ChevronLeft";
};

module ChevronDown = {
  [@bs.module "react-feather"] [@react.component]
  external make:
    (~className: string=?, ~style: ReactDOMRe.Style.t=?) => React.element =
    "ChevronDown";
};

type simpleIcon = {
  svg: string,
  title: string,
  hex: string,
  path: string,
};

module SimpleIcon = (IconData: {let icon: simpleIcon;}) => {
  [@react.component]
  let make =
      (
        ~height="24",
        ~width="24",
        ~className="",
        ~style=ReactDOMRe.Style.make(),
        ~ariaLabel=IconData.icon.title ++ " Icon",
        ~ariaHidden=false,
      ) =>
    <svg
      role="img"
      viewBox="0 0 24 24"
      height
      width
      className
      style=ReactDOMRe.Style.(
        make(~fill="#" ++ IconData.icon.hex, ())->combine(style)
      )
      ariaLabel
      ariaHidden>
      <path d={IconData.icon.path} />
    </svg>;
  React.setDisplayName(make, IconData.icon.title);
};

module Reason =
  SimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/reason";
  });

module React =
  SimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/react";
  });

module D3 =
  SimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/d3-dot-js";
  });

module GitHub =
  SimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/github";
  });

module NodeJs =
  SimpleIcon({
    [@bs.module] external icon: simpleIcon = "simple-icons/icons/node-dot-js";
  });
