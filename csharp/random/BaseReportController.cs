using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Configuration;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;
using EPIC.Reporting.Models;
using EPIC.Reporting.Security;
using Firepig.Analysis.Data;

namespace EPIC.Reporting.Controllers
{
    public abstract class BaseReportController : Controller
    {
        protected IList<ISpanningMember> GetMembers(string mdx)
        {
            QueryBuilder qry = new QueryBuilder();
            qry.AddAxis(new IQueryAxisItem[] {
                new QueryBuilderLiteral(mdx)
            });
            qry.Warehouse = "[Epic Warehouse]";
            QueryContext ctx = new QueryContext();
            ICellDataSource eth = qry.ExecuteDataSource(ctx);
            IList<ISpanningMember> members = new List<ISpanningMember>();
            foreach (var row in eth.Columns)
            {
                if (row.Members.Count > 0)
                {
                    members.Add(row.Members[0]);
                }
            }
            return members;
        }
    }

    public class SchoolAccessFilterAttribute : ActionFilterAttribute
    {
        IisTraceListener iisTraceListener = new IisTraceListener();

        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            iisTraceListener.WriteLine("OnActionExecuting");
            iisTraceListener.Flush();

            HttpRequestBase request = filterContext.HttpContext.Request;
            HttpResponseBase response = filterContext.HttpContext.Response;
            HttpSessionStateBase session = filterContext.HttpContext.Session;

            RouteData routeData = filterContext.RouteData;
            RouteValueDictionary routeDataValues = routeData.Values;

            string token = null;

            if (IsIntegratedLogin() && (request.QueryString["token"] != null))
            { token = request.QueryString["token"]; }
            else
            { token = "9"; }

            Authenticate(ref session, token);

            if (!IsAuthenticated(session))
            { Login(ref response); }

            if (IsDashboardRequest(filterContext))
            { Dashboard(ref filterContext, session); }

            if (!IsAuthorized(session, request, routeDataValues))
            { Unauthorized(ref filterContext); }

            base.OnActionExecuting(filterContext);
        }

        private void Authenticate(ref HttpSessionStateBase session, string token)
        {
            iisTraceListener.WriteLine("Authenticate");
            iisTraceListener.Flush();

            User user = new User();
            UserAuthentication userAuthentication = new UserAuthentication();

            if (userAuthentication.IsAuthenticated(ref user, token))
            { session.Contents.Add("USER", user); }
            else
            { session.Contents.Remove("USER"); }
        }

        private bool IsAuthenticated(HttpSessionStateBase session)
        {
            iisTraceListener.WriteLine("IsAuthenticated");
            iisTraceListener.Flush();

            if ((session != null) && (session["USER"] != null))
            { return true; }

            return false;
        }

        private bool IsAuthorized(HttpSessionStateBase session, HttpRequestBase request, RouteValueDictionary routeDataValues)
        {
            iisTraceListener.WriteLine("IsAuthorized");
            iisTraceListener.Flush();

            bool isAuthorized = false;

            if (routeDataValues["schoolId"] == null)
            { return false; }

            string schoolIdString = routeDataValues["schoolId"] as string;
            schoolIdString = schoolIdString.Trim().ToLower();

            if ((schoolIdString == "dashboard") || (schoolIdString == "error") || (schoolIdString == "login.aspx"))
            { return true; }

            int schoolIdInt = 0;

            if (!int.TryParse(schoolIdString, out schoolIdInt))
            { return false; }

            if (schoolIdInt == 0)
            { return false; }

            User user = session["USER"] as User;

            if (user.Schools == null)
            { user.Schools = UserAuthentication.GetSchools(user.Email); }

            foreach (School school in user.Schools)
            {
                if (schoolIdInt == school.SchoolId)
                {
                    isAuthorized = true;
                    break;
                }
            }

            user.AdminPeriods = UserAuthentication.GetAdminPeriodsBySchoolId(schoolIdInt);

            if (session["ADMINPERIOD"] == null)
            { session.Add("ADMINPERIOD", user.AdminPeriods.First()); }

            if (request.QueryString["adminPeriodId"] != null)
            {
                string adminPeriodIdString = request.QueryString["adminPeriodId"] as string;
                adminPeriodIdString = adminPeriodIdString.Trim();

                int adminPeriodIdInt = 0;

                if (int.TryParse(adminPeriodIdString, out adminPeriodIdInt))
                {
                    if (adminPeriodIdInt != 0)
                    {
                        foreach (AdminPeriod adminPeriod in user.AdminPeriods)
                        {
                            if (adminPeriodIdInt == adminPeriod.AdminPeriodId)
                            {
                                session.Add("ADMINPERIOD", adminPeriod);
                                break;
                            }
                        }
                    }
                }
            }

            return isAuthorized;
        }

        private bool IsDashboardRequest(ActionExecutingContext filterContext)
        {
            iisTraceListener.WriteLine("IsDashboardRequest");
            iisTraceListener.Flush();

            RouteData data = filterContext.RouteData;
            RouteValueDictionary values = data.Values;

            string schoolId = null;

            if (values["schoolId"] != null)
            {
                schoolId = values["schoolId"] as string;

                int schoolIdParsed = 0;

                if (!int.TryParse(schoolId, out schoolIdParsed))
                {
                    iisTraceListener.WriteLine(schoolId);
                    iisTraceListener.Flush();

                    if (schoolId.ToString().Trim().ToLower() == "dashboard")
                    { return true; }
                }
            }

            return false;
        }

        private bool IsIntegratedLogin()
        { return Convert.ToBoolean((ConfigurationManager.AppSettings["IntegratedLogin"] as string)); }

        private void Dashboard(ref ActionExecutingContext filterContext, HttpSessionStateBase session)
        {
            iisTraceListener.WriteLine("Dashboard");
            iisTraceListener.Flush();

            User user = session["USER"] as User;

            iisTraceListener.WriteLine((user == null));
            iisTraceListener.Flush();

            filterContext.Result = new RedirectToRouteResult
            (
                new RouteValueDictionary
                (
                    new
                    {
                        controller = "Dashboard",
                        action = "Index",
                        schoolId = user.Schools.First().SchoolId.ToString()
                    }
                )
            );
        }

        private void Login(ref HttpResponseBase response)
        {
            iisTraceListener.WriteLine("Login");
            iisTraceListener.WriteLine(ConfigurationManager.AppSettings["RemoteLoginUrl"]);
            iisTraceListener.Flush();

            response.Redirect((ConfigurationManager.AppSettings["RemoteLoginUrl"] as string), true);
        }

        private void Unauthorized(ref ActionExecutingContext filterContext)
        {
            iisTraceListener.WriteLine("Unauthorized");
            iisTraceListener.Flush();

            UrlHelper url = new UrlHelper(filterContext.RequestContext);
            String action = url.Action("Unauthorized", "Error");
            filterContext.Result = new RedirectResult(action);
        }
    }
}